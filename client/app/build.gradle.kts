plugins {
    id("org.jetbrains.kotlin.jvm") version "1.9.10"
    application
}

repositories {
    mavenCentral()
}

application {
    mainClass = "pl.smq.example.AppKt"
}

dependencies {
    implementation(project(":lib"))
    implementation(kotlin("stdlib"))
}

sourceSets {
    main {
        java {
            srcDir("src/main/kotlin")
        }
    }
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

tasks.build {
    dependsOn(":lib:build")
}


tasks.jar {
    duplicatesStrategy = DuplicatesStrategy.EXCLUDE
    from(sourceSets.main.get().output)
    dependsOn(configurations.runtimeClasspath)
    manifest {
        attributes["Main-Class"] = "pl.smq.example.AppKt"
        attributes["Class-Path"] = configurations.runtimeClasspath.get().asPath.split(":").joinToString(" ")
    }
    from({
        configurations.runtimeClasspath.get().filter { it.name.endsWith("jar") }.map { zipTree(it) }
    })
}
