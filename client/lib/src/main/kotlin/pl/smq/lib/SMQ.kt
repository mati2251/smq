package pl.smq.lib

import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.channels.BufferOverflow
import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.PrintWriter
import java.net.Socket

@DelicateCoroutinesApi
class SMQ(host: String, port: Int) {
    private val socket: Socket = Socket(host, port)
    private val writer: PrintWriter = PrintWriter(socket.getOutputStream(), true)
    private val reader: BufferedReader = BufferedReader(InputStreamReader(socket.getInputStream()))
    fun messageQueue(
        topic: String, bufferSize: Int = 100,
        onOverflowBuffer: BufferOverflow = BufferOverflow.SUSPEND,
    ): MessageQueue {
        return MessageQueue(writer, reader, topic, bufferSize, onOverflowBuffer)
    }

    companion object {
        var requestCounter = 1
    }
}
