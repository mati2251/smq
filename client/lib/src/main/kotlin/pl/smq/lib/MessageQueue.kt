package pl.smq.lib

import java.io.PrintWriter
import kotlinx.coroutines.*
import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.flow.*
import pl.smq.lib.exceptions.FullMessageBufferException
import pl.smq.lib.exceptions.MessageQueueException
import pl.smq.lib.models.Request
import pl.smq.lib.models.RequestType
import pl.smq.lib.models.Response

@DelicateCoroutinesApi
class MessageQueue(
        private val writer: PrintWriter,
        private val topic: String,
        bufferSize: Int = 100,
        onOverflowBuffer: BufferOverflow = BufferOverflow.SUSPEND,
) {
    private val action: TopicUtil = TopicUtil(writer, topic)
    private var isSubscriber: Boolean = false
    private var isPublisher: Boolean = false
    private val messages: Channel<String> = Channel(bufferSize, onOverflowBuffer)
    private var readerJob: Job? = null
    private val messageListener: MutableList<(String) -> Unit> = mutableListOf()

    suspend fun registerAsSubscriber(): Response {
        if (isSubscriber) throw MessageQueueException("You are already a subscriber")
        if (messages.isClosedForSend) throw FullMessageBufferException()
        this.isSubscriber = true
        val requestId = action.registerAsSubscriber()
        return SMQ.responses.filter { it.requestId == requestId }.take(1).single()
    }

    suspend fun unregisterAsSubscriber(): Response {
        if (!isSubscriber) throw MessageQueueException("You are not a subscriber")
        if (messages.isClosedForSend) throw FullMessageBufferException()
        this.isSubscriber = false
        val requestId = action.unregisterAsSubscriber()
        return SMQ.responses.filter { it.requestId == requestId }.take(1).single()
    }

    suspend fun registerAsPublisher(): Response {
        if (isPublisher) throw MessageQueueException("You are already a publisher")
        if (messages.isClosedForSend) throw FullMessageBufferException()
        this.isPublisher = true
        val requestId = action.registerAsPublisher()
        return SMQ.responses.filter { it.requestId == requestId }.take(1).single()
    }

    suspend fun unregisterAsPublisher(): Response {
        if (!isPublisher) throw MessageQueueException("You are not a publisher")
        if (messages.isClosedForSend) throw FullMessageBufferException()
        this.isPublisher = false
        val requestId = action.unregisterAsPublisher()
        return SMQ.responses.filter { it.requestId == requestId }.take(1).single()
    }

    suspend fun sendMessage(content: String): Response {
        if (!isPublisher)
                throw MessageQueueException("Cannot send message, you are not a publisher")
        if (messages.isClosedForSend) throw FullMessageBufferException()
        val requestId = SMQ.requestCounter++
        val request = Request(RequestType.MESSAGE, requestId, topic, content)
        writer.println(request.serialize())
        return SMQ.responses.filter { it.requestId == requestId }.take(1).single()
    }

    suspend fun readMessages(): String {
        return this.messages.receive()
    }

    suspend fun addMessage(message: String){
        if (!isSubscriber)
                throw MessageQueueException("You received inlegal message, you are not a subscriber")
        if (messages.isClosedForSend) throw FullMessageBufferException()
        messages.send(message)
        this.notifyMessageListeners(message)
    }

    private fun notifyMessageListeners(message: String) {
        messageListener.forEach { it(message) }
    }

    fun addMessageListener(listener: (String) -> Unit) {
        messageListener.add(listener)
    }

    fun removeMessageListener(listener: (String) -> Unit) {
        messageListener.remove(listener)
    }
}
