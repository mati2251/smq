plugins {
    id("org.jetbrains.kotlin.jvm") version "1.9.10"
    application
}

repositories {
    mavenLocal()
    mavenCentral()
}

dependencies {
    implementation("pl.smq:smq-lib:0.0.1")
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