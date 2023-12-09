package pl.smq.lib

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import kotlinx.serialization.json.Json
import kotlinx.serialization.json.encodeToJsonElement
import pl.smq.lib.models.Message
import pl.smq.lib.models.RequestType
import java.io.BufferedReader
import java.io.PrintWriter

class MessageQueue(private val writer: PrintWriter, private val reader: BufferedReader, private val topic: String) {
    private val action: TopicUtil = TopicUtil(writer, topic)

    private val messageListener: MutableList<(Message) -> Unit> = mutableListOf()

    init {
        Thread {
            while (true) {
                val buffer = reader.readLine()
                val message = Json.decodeFromString(Message.serializer(), buffer)
                messageListener.forEach { it(message) }
            }
        }.start()
    }

    fun registerAsSubscriber() {
        action.registerAsSubscriber()

    }

    fun unregisterAsSubscriber() {
        action.unregisterAsSubscriber()
    }

    fun registerAsPublisher() {
        action.registerAsPublisher()
    }

    fun unregisterAsPublisher() {
        action.unregisterAsPublisher()
    }

    fun sendMessage(content: String) {
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
        return withContext(Dispatchers.IO) {
            val buffer = reader.readLine()
            return@withContext buffer
        }
    }
}
