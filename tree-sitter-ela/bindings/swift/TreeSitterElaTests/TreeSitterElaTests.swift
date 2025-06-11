import XCTest
import SwiftTreeSitter
import TreeSitterEla

final class TreeSitterElaTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_ela())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Ela grammar")
    }
}
