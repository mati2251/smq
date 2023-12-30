package pl.smq.lib

import kotlinx.coroutines.*
import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.flow.*
import pl.smq.lib.exceptions.FullMessageBufferException
import pl.smq.lib.exceptions.MessageQueueException
import pl.smq.lib.models.ExchangeType
import pl.smq.lib.models.Request
import pl.smq.lib.models.RequestType
import pl.smq.lib.models.Response
import java.io.BufferedReader
import java.io.PrintWriter


@DelicateCoroutinesApi
class MessageQueue(
    private val writer: PrintWriter,
    private val reader: BufferedReader,
    private val topic: String,
    bufferSize: Int = 100,
    onOverflowBuffer: BufferOverflow = BufferOverflow.SUSPEND,
) {
    private val action: TopicUtil = TopicUtil(writer, topic)
    private var isSubscriber: Boolean = false
    private var isPublisher: Boolean = false
    private val messages: Channel<String> = Channel(bufferSize, onOverflowBuffer)
    private val responses: MutableSharedFlow<Response> = MutableSharedFlow(replay = 1, onBufferOverflow = BufferOverflow.DROP_OLDEST)
    private var readerJob: Job? = null
    private val messageListener: MutableList<(String) -> Unit> = mutableListOf()

    suspend fun registerAsSubscriber(): Response {
        if (isSubscriber)
            throw MessageQueueException("You are already a subscriber")
        if (messages.isClosedForSend)
            throw FullMessageBufferException()
        if (readerJob == null)
            startReading()
        this.isSubscriber = true
        val requestId = action.registerAsSubscriber()
        return this.responses.filter { it.requestId == requestId }.take(1).single()
    }

    suspend fun unregisterAsSubscriber(): Response {
        if (!isSubscriber)
            throw MessageQueueException("You are not a subscriber")
        if (messages.isClosedForSend)
            throw FullMessageBufferException()
        if (readerJob != null && !isPublisher)
            stopReading()
        this.isSubscriber = false
        val requestId = action.unregisterAsSubscriber()
        return this.responses.filter { it.requestId == requestId }.take(1).single()
    }

    suspend fun registerAsPublisher(): Response {
        if (isPublisher)
            throw MessageQueueException("You are already a publisher")
        if (messages.isClosedForSend)
            throw FullMessageBufferException()
        if (readerJob == null)
            startReading()
        this.isPublisher = true
        val requestId = action.registerAsPublisher()
        return this.responses.filter { it.requestId == requestId }.take(1).single()
    }

    suspend fun unregisterAsPublisher(): Response {
        if (!isPublisher)
            throw MessageQueueException("You are not a publisher")
        if (messages.isClosedForSend)
            throw FullMessageBufferException()
        if (readerJob != null && !isSubscriber)
            stopReading()
        this.isPublisher = false
        val requestId = action.unregisterAsPublisher()
        return this.responses.filter { it.requestId == requestId }.take(1).single()
    }

    suspend fun sendMessage(content: String): Response {
        if (!isPublisher)
            throw MessageQueueException("Cannot send message, you are not a publisher")
        if (messages.isClosedForSend)
            throw FullMessageBufferException()
        val requestId = SMQ.requestCounter++
        val request = Request(RequestType.MESSAGE, requestId, topic, content)
        writer.println(request.serialize())
        return this.responses.filter { it.requestId == requestId }.take(1).single()
    }

    suspend fun readMessages(): String {
        return this.messages.receive()
    }

    fun addMessageListener(listener: (String) -> Unit) {
        messageListener.add(listener)
    }

    fun removeMessageListener(listener: (String) -> Unit) {
        messageListener.remove(listener)
    }

    private fun startReading() {
        this.readerJob = CoroutineScope(Dispatchers.IO).launch {
            var buf = ""
            while (true) {
                if (!buf.contains("\n\n"))
                    buf += reader.readLine() + "\n"
                if (ExchangeUtils.isWholeExchange(buf)) {
                    val (exchange, rest) = ExchangeUtils.splitExchange(buf)
                    buf = rest
                    when (ExchangeUtils.checkType(exchange)) {
                        ExchangeType.RESPONSE -> {
                            val response = ExchangeUtils.deserializeResponse(exchange)
                            this@MessageQueue.responses.emit(response)
                        }

                        ExchangeType.REQUEST -> {
                            val request = ExchangeUtils.deserializeRequest(exchange)
                            if (request.type == RequestType.MESSAGE && isSubscriber)
                                this@MessageQueue.messages.send(request.body)
                            messageListener.forEach { it(request.body) }
                        }
                    }
                }
            }
        }
    }

    private fun stopReading() {
        this.readerJob?.cancel()
    }
}
