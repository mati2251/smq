package pl.smq.lib

import kotlinx.serialization.json.Json
import kotlinx.serialization.json.encodeToJsonElement
import pl.smq.lib.models.ActionTopic
import pl.smq.lib.models.ActionTopicType
import pl.smq.lib.models.RequestType
import java.io.PrintWriter

class TopicUtil(private val writer: PrintWriter) {
    private fun action(actionTopic: ActionTopic) {
        val json = Json.encodeToJsonElement(actionTopic)
        writer.println("${RequestType.ACTION}\n$json")
    }

    fun subscribe(topic: String) {
        action(ActionTopic(ActionTopicType.SUBSCRIBE, topic))
    }

    fun unsubscribe(topic: String) {
        action(ActionTopic(ActionTopicType.UNSUBSCRIBE, topic))
    }

    fun registerAsPublisher(topic: String) {
        action(ActionTopic(ActionTopicType.PUBLISH, topic))
    }

    fun unregisterAsPublisher(topic: String) {
        action(ActionTopic(ActionTopicType.UNPUBLISH, topic))
    }
}
