// swift-tools-version:5.3

import Foundation
import PackageDescription

var sources = ["src/parser.c"]
if FileManager.default.fileExists(atPath: "src/scanner.c") {
    sources.append("src/scanner.c")
}

let package = Package(
    name: "TreeSitterEla",
    products: [
        .library(name: "TreeSitterEla", targets: ["TreeSitterEla"]),
    ],
    dependencies: [
        .package(url: "https://github.com/tree-sitter/swift-tree-sitter", from: "0.8.0"),
    ],
    targets: [
        .target(
            name: "TreeSitterEla",
            dependencies: [],
            path: ".",
            sources: sources,
            resources: [
                .copy("queries")
            ],
            publicHeadersPath: "bindings/swift",
            cSettings: [.headerSearchPath("src")]
        ),
        .testTarget(
            name: "TreeSitterElaTests",
            dependencies: [
                "SwiftTreeSitter",
                "TreeSitterEla",
            ],
            path: "bindings/swift/TreeSitterElaTests"
        )
    ],
    cLanguageStandard: .c11
)
