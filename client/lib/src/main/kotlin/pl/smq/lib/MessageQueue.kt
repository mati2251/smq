package pl.smq.lib

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import kotlinx.serialization.json.Json
import kotlinx.serialization.json.encodeToJsonElement
import pl.smq.lib.models.Message
import pl.smq.lib.models.RequestType
import java.io.BufferedReader
import java.io.PrintWriter
import java.lang.RuntimeException

class MessageQueueException(message: String) : RuntimeException(message)

class MessageQueue(private val writer: PrintWriter, private val reader: BufferedReader, private val topic: String) {
    private val action: TopicUtil = TopicUtil(writer, topic)
    private var isSubscriber: Boolean = false
    private var isPublisher: Boolean = false
    private var thread: Thread = Thread {
        while (true) {
            val c = CharArray(100)
            val size = reader.read(c)
            if (size == -1)
                continue
            val buffer = String(c, 0, size)
            println(buffer)
        }
    }

    private val messageListener: MutableList<(Message) -> Unit> = mutableListOf()

    fun registerAsSubscriber() {
        if (isSubscriber)
            throw MessageQueueException("You are already a subscriber")
        action.registerAsSubscriber()
        thread.start()
        this.isSubscriber = true;
    }

    fun unregisterAsSubscriber() {
        if (!isSubscriber)
            throw MessageQueueException("You are not a subscriber")
        action.unregisterAsSubscriber()
        this.isSubscriber = false;
        thread.interrupt();
    }

    fun registerAsPublisher() {
        if (isPublisher)
            throw MessageQueueException("You are already a publisher")
        action.registerAsPublisher()
        this.isPublisher = true;
    }

    fun unregisterAsPublisher() {
        if (!isPublisher)
            throw MessageQueueException("You are not a publisher")
        action.unregisterAsPublisher()
        this.isPublisher = false;
    }

    fun sendMessage(content: String) {
        if (!isPublisher)
            throw MessageQueueException("Cannot send message, you are not a publisher")
        val message = Message(topic, content)
        val json = Json.encodeToJsonElement(message)
        writer.println("${RequestType.MESSAGE}\n$json")
    }

    fun addMessageListener(listener: (Message) -> Unit) {
        messageListener.add(listener)
    }

    fun removeMessageListener(listener: (Message) -> Unit) {
        messageListener.remove(listener)
    }

    suspend fun readMessage(): String {
        if (!isSubscriber)
            throw MessageQueueException("Cannot read message, you are not a subscriber")
        thread.interrupt()
        return withContext(Dispatchers.IO) {
            val buffer = reader.readLine()
            thread.start()
            return@withContext buffer
        }
    }
}
