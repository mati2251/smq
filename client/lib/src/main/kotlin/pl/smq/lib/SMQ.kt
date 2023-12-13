package pl.smq.lib

import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.PrintWriter
import java.net.Socket

class SMQ(host: String, port: Int) {
    private val socket: Socket = Socket(host, port)
    private val writer: PrintWriter = PrintWriter(socket.getOutputStream(), true)
    private val reader: BufferedReader = BufferedReader(InputStreamReader(socket.getInputStream()))

    fun messageQueue(topic: String): MessageQueue {
        return MessageQueue(writer, reader, topic)
    }
}
