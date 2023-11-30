import org.jetbrains.kotlin.gradle.tasks.KotlinCompile

val ktlint by configurations.creating

plugins {
    kotlin("jvm") version "1.9.10"
}

repositories {
    mavenCentral()
}

dependencies {
    ktlint("com.pinterest.ktlint:ktlint-cli:1.0.1") {
        attributes {
            attribute(Bundling.BUNDLING_ATTRIBUTE, objects.named(Bundling.EXTERNAL))
        }
    }
}

tasks.withType<KotlinCompile> {
    kotlinOptions {
        freeCompilerArgs += "-Xjsr305=strict"
        jvmTarget = "20"
    }
}

tasks.register<JavaExec>("ktlintCheck") {
    group = LifecycleBasePlugin.VERIFICATION_GROUP
    description = "Check Kotlin code style"
    classpath = ktlint
    mainClass.set("com.pinterest.ktlint.Main")
    jvmArgs("--add-opens=java.base/java.lang=ALL-UNNAMED")
    args(
        "app/**/*.kt",
        "lib/**/*.kt",
        "**.kts",
        "--reporter=plain",
        "--editorconfig=.editorconfig",
    )
}

tasks.register<JavaExec>("ktlintFormat") {
    group = LifecycleBasePlugin.VERIFICATION_GROUP
    description = "Save ktlint scanning results to report.sarif"
    classpath = ktlint
    mainClass.set("com.pinterest.ktlint.Main")
    jvmArgs("--add-opens=java.base/java.lang=ALL-UNNAMED")
    args(
        "-F",
        "app/**/*.kt",
        "lib/**/*.kt",
        "**.kts",
        "--reporter=plain",
        "--editorconfig=.editorconfig",
    )
}

tasks.register<JavaExec>("ktlintReport") {
    group = LifecycleBasePlugin.VERIFICATION_GROUP
    description = "Save ktlint scanning results to report.sarif"
    classpath = ktlint
    mainClass.set("com.pinterest.ktlint.Main")
    jvmArgs("--add-opens=java.base/java.lang=ALL-UNNAMED")
    args(
        "app/**/*.kt",
        "lib/**/*.kt",
        "**.kts",
        "--reporter=sarif,output=report.sarif",
        "--editorconfig=.editorconfig",
    )
}
