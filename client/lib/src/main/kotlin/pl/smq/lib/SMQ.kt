package pl.smq.lib

import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.channels.BufferOverflow
import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.PrintWriter
import java.net.Socket
import pl.smq.lib.models.Response
import pl.smq.lib.models.ExchangeType
import pl.smq.lib.models.RequestType
import kotlinx.coroutines.flow.*
import kotlinx.coroutines.*

@DelicateCoroutinesApi
class SMQ(private val host: String, private val port: Int) {
    private lateinit var socket: Socket
    private lateinit var writer: PrintWriter
    private lateinit var reader: BufferedReader
    private var readerJob: Job? = null
    private val queues: MutableMap<String, MessageQueue> = mutableMapOf()

    fun connect(){
        this.socket = Socket(host, port)
        this.writer = PrintWriter(socket.getOutputStream(), true)
        this.reader = BufferedReader(InputStreamReader(socket.getInputStream()))
        startReading()
    }

    fun disconnect(){
        this.socket.close()
        this.writer.close()
        this.reader.close()
        this.readerJob?.cancel()
    }

    fun messageQueue(
        topic: String, bufferSize: Int = 100,
        onOverflowBuffer: BufferOverflow = BufferOverflow.SUSPEND,
    ): MessageQueue {
        if (queues.containsKey(topic)) {
            return queues[topic]!!
        }
        val queue = MessageQueue(writer, topic, bufferSize, onOverflowBuffer)
        queues[topic] = queue
        return queue
    }

    companion object {
        var requestCounter = 1
        val responses: MutableSharedFlow<Response> = MutableSharedFlow(replay = 1, onBufferOverflow = BufferOverflow.DROP_OLDEST)
    }

    private fun startReading() {
        this.readerJob = CoroutineScope(Dispatchers.IO).launch {
            var buf = ""
            while (true) {
                val cbuf = CharArray(512)
                val size = reader.read(cbuf, 0, 512)
                if (size == -1) continue
                buf += cbuf.joinToString("").substring(0, size)
                if (ExchangeUtils.isWholeExchange(buf)) {
                    val (exchange, rest) = ExchangeUtils.splitExchange(buf)
                    buf = rest
                    when (ExchangeUtils.checkType(exchange)) {
                        ExchangeType.RESPONSE -> {
                            val response = ExchangeUtils.deserializeResponse(exchange)
                            responses.emit(response)
                        }

                        ExchangeType.REQUEST -> {
                            val request = ExchangeUtils.deserializeRequest(exchange)
                            if (request.type == RequestType.MESSAGE) {
                                queues[request.topic]?.addMessage(request.body)
                            }
                        }
                    }
                }
            }
        }
    }
}
