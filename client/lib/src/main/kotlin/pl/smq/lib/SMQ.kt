package pl.smq.lib

import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.PrintWriter
import java.net.Socket

class SMQ(host: String, port: Int) {
    private val socket: Socket = Socket(host, port)
    private val writer: PrintWriter = PrintWriter(socket.getOutputStream(), true)
    private val action: TopicUtil = TopicUtil(writer)
    private val reader: BufferedReader = BufferedReader(InputStreamReader(socket.getInputStream()))

    fun subscribe(topic: String) {
        action.subscribe(topic)
    }

    fun unsubscribe(topic: String) {
        action.unsubscribe(topic)
    }

    fun publisher(topic: String) {
        action.registerAsPublisher(topic)
    }

    fun unpublisher(topic: String) {
        action.unregisterAsPublisher(topic)
    }
}
