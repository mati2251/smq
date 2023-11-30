plugins {
    id("org.gradle.toolchains.foojay-resolver-convention") version "0.7.0"
}

rootProject.name = "smq"
include("app", "lib", "build.gradle.kts")
