package pl.smq.lib

import java.io.PrintWriter
import kotlinx.coroutines.*
import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.flow.*
import pl.smq.lib.exceptions.FullMessageBufferException
import pl.smq.lib.exceptions.MessageQueueException
import pl.smq.lib.exceptions.ResponseErrorException
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
    private val messageListener: MutableList<(String) -> Unit> = mutableListOf()

    suspend fun registerAsSubscriber(): Response {
        if (isSubscriber) throw MessageQueueException("You are already a subscriber")
        if (messages.isClosedForSend) throw FullMessageBufferException()
        this.isSubscriber = true
        val requestId = action.registerAsSubscriber()
        val res = SMQ.responses.filter { it.requestId == requestId }.take(1).single()
        if (res.code != 0){
            throw ResponseErrorException(res)
        }
        return res
    }

    suspend fun unregisterAsSubscriber(): Response {
        if (!isSubscriber) throw MessageQueueException("You are not a subscriber")
        if (messages.isClosedForSend) throw FullMessageBufferException()
        this.isSubscriber = false
        val requestId = action.unregisterAsSubscriber()
        val res = SMQ.responses.filter { it.requestId == requestId }.take(1).single()
        if (res.code != 0){
            throw ResponseErrorException(res)
        }
        return res
    }

    suspend fun registerAsPublisher(): Response {
        if (isPublisher) throw MessageQueueException("You are already a publisher")
        if (messages.isClosedForSend) throw FullMessageBufferException()
        this.isPublisher = true
        val requestId = action.registerAsPublisher()
        val res = SMQ.responses.filter { it.requestId == requestId }.take(1).single()
        if (res.code != 0){
            throw ResponseErrorException(res)
        }
        return res
    }

    suspend fun unregisterAsPublisher(): Response {
        if (!isPublisher) throw MessageQueueException("You are not a publisher")
        if (messages.isClosedForSend) throw FullMessageBufferException()
        this.isPublisher = false
        val requestId = action.unregisterAsPublisher()
        val res = SMQ.responses.filter { it.requestId == requestId }.take(1).single()
        if (res.code != 0){
            throw ResponseErrorException(res)
        }
        return res
    }

    suspend fun sendMessage(content: String): Response {
        if (!isPublisher)
                throw MessageQueueException("Cannot send message, you are not a publisher")
        if (messages.isClosedForSend) throw FullMessageBufferException()
        val requestId = SMQ.requestCounter++
        val request = Request(RequestType.MESSAGE, requestId, topic, content)
        writer.println(request.serialize())
        val res = SMQ.responses.filter { it.requestId == requestId }.take(1).single()
        if (res.code != 0){
            throw ResponseErrorException(res)
        }
        return res
    }

    suspend fun readMessage(): String {
        return this.messages.receive()
    }

    suspend fun addMessage(message: String){
        if (!isSubscriber)
            return
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
