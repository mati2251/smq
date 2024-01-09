package pl.smq.example

import pl.smq.example.publishers.*

object PublisherFactory {
    fun get(testCase: Int): Action {
        when(testCase){
            1 -> Publisher()
        }
        return Publisher()
    }
}
