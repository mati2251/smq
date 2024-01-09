package pl.smq.example

import pl.smq.example.subscribers.*

object SubscriberFactory {
    fun get(testCase: Int): Action {
        when(testCase){
            1 -> Subscriber()
        }
        return Subscriber()
    }
}
