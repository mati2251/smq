package pl.smq.lib

import kotlinx.coroutines.DelicateCoroutinesApi
import pl.smq.lib.models.ActionTopicType
import pl.smq.lib.models.Request
import pl.smq.lib.models.RequestType
import java.io.PrintWriter

class TopicUtil(private val writer: PrintWriter, private val topic: String) {
    @OptIn(DelicateCoroutinesApi::class)
    private fun action(actionTopic: ActionTopicType): Int {
        val requestId = SMQ.requestCounter++
        val request = Request(RequestType.ACTION, requestId, topic, actionTopic.toString())
        writer.println(request.serialize())
        return requestId
    }

    fun registerAsSubscriber(): Int {
        return action(ActionTopicType.SUBSCRIBE)
    }

    fun unregisterAsSubscriber(): Int {
        return action(ActionTopicType.UNSUBSCRIBE)
    }

    fun registerAsPublisher(): Int {
        return action(ActionTopicType.PUBLISH)
    }

    fun unregisterAsPublisher(): Int {
        return action(ActionTopicType.UNPUBLISH)
    }
}
