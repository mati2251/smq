package pl.smq.example

fun main(args: Array<String>) {
    if (args.isEmpty()) {
        println("Usage: <pub|sub>")
        return
    }
    val command: Subcommand = when (args[0]) {
        "pub" -> Publisher()
        "sub" -> Subscriber()
        else -> {
            println("Unknown command")
            return
        }
    }
    command.execute()
}
