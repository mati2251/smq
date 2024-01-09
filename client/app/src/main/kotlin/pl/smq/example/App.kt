package pl.smq.example

suspend fun main(args: Array<String>) {
    if (args.size < 5) {
       printHelp()
        return
    }
    val testCase: Int
    val host: String = args[2];
    val topic: String = args[4]
    val port: Int
    try{
        testCase = args[1].toInt()
        port = args[3].toInt()
    }
    catch (_: Exception){
        printHelp()
        return
    }
    val command: Action = when (args[0]) {
        "pub" -> PublisherFactory.get(testCase)
        "sub" -> SubscriberFactory.get(testCase)
        else -> {
            printHelp()
            return
        }
    }
    command.execute(host, port, topic)
}

fun printHelp(){
    println("Usage: <pub|sub> test-case host port topic")
    println("""
            Subscriber test cases:
            1. Normal subscriber 
            Publisher test cases:
            1. Normal publisher
        """.trimIndent())
    return
}
