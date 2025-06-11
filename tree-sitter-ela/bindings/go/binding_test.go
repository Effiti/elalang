package tree_sitter_ela_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_ela "github.com/effiti/elalang/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_ela.Language())
	if language == nil {
		t.Errorf("Error loading Ela grammar")
	}
}
