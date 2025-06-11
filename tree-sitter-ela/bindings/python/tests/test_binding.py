from unittest import TestCase

import tree_sitter
import tree_sitter_ela


class TestLanguage(TestCase):
    def test_can_load_grammar(self):
        try:
            tree_sitter.Language(tree_sitter_ela.language())
        except Exception:
            self.fail("Error loading Ela grammar")
