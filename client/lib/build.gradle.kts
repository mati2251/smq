import java.net.URI

plugins {
    id("org.jetbrains.kotlin.jvm") version "1.9.10"
    `maven-publish`
    `java-library`
}

repositories {
    mavenCentral()
}

dependencies {
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

publishing {
    publications {
        create<MavenPublication>("maven") {
            groupId = "pl.smq"
            artifactId = "smq-lib"
            version = "0.0.1"

            from(components["java"])
        }
    }
    repositories {
        maven {
            name = "GitHubPackages"
            url = URI("https://maven.pkg.github.com/mati2251/smq")
            credentials {
                username = System.getenv("GITHUB_ACTOR")
                password = System.getenv("GITHUB_TOKEN")
            }
        }
    }
}

tasks.check {
    dependsOn(tasks.getByPath(":ktlintCheck"))
}
