@file:OptIn(ExperimentalCli::class)
package pl.smq.example

import kotlinx.cli.*

@OptIn(ExperimentalCli::class)
fun main(args: Array<String>) {
    val parser = ArgParser("SMQ client example")
    val subscriber = Subscriber()
    val publisher = Publisher()
    parser.subcommands(subscriber, publisher)
    parser.parse(args)
}
