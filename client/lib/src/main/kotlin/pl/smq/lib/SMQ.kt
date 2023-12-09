package pl.smq.lib

import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.PrintWriter
import java.net.Socket

class SMQ(host: String, port: Int) {
    private val socket: Socket = Socket(host, port)
    private val output: PrintWriter = PrintWriter(socket.getOutputStream(), true)
    private val input: BufferedReader = BufferedReader(InputStreamReader(socket.getInputStream()))
}
