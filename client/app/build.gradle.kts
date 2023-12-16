plugins {
    id("org.jetbrains.kotlin.jvm") version "1.9.10"
    application
}

repositories {
    mavenLocal()
    mavenCentral()
}

dependencies {
    implementation(project(":lib"))
    implementation("org.jetbrains.kotlinx:kotlinx-cli:0.3.6")
}

testing {
    suites {
        val test by getting(JvmTestSuite::class) {
            useKotlinTest("1.9.10")
        }
    }
}

java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(20))
    }
}

application {
    mainClass.set("pl.smq.example.AppKt")
}

tasks.jar{
    manifest {
        attributes["Main-Class"] = "pl.smq.example.AppKt"
        attributes["Class-Path"] = configurations.runtimeClasspath.get().asPath.split(":").joinToString(" ")
    }
}
