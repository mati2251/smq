package pl.smq.example

import pl.smq.example.publishers.*

object PublisherFactory {
    fun get(testCase: Int): Action {
        return when(testCase){
            1 -> Publisher()
            2 -> PublisherOnce()
            else -> Publisher()
        }
    }
}
