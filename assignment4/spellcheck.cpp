#include "spellcheck.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>

template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

Corpus tokenize(std::string& source) {
  /* TODO: Implement this method */

  std::vector<std::string::iterator> space_list = find_all(source.begin(), source.end(), [](unsigned char c) -> int { return std::isspace(c); });

  std::set<Token> tokens;
  std::transform(space_list.begin(), 
                space_list.end() - 1, 
                space_list.begin() + 1, 
                std::inserter(tokens, tokens.end()), 
                [&source](std::string::iterator it1, std::string::iterator it2) -> Token {
                  return Token(source, it1, it2);
                }
  );

  std::erase_if(tokens, [](Token token) -> bool { return token.content.empty(); });

  return tokens;
}

std::set<Misspelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
  /* TODO: Implement this method */

  namespace rv = std::ranges::views;

  auto is_misspelled = [&dictionary](const Token& token) -> bool {
    return !dictionary.contains(token.content);
  };

  auto to_misspelling = [&dictionary](const Token& token) -> Misspelling {
    
    auto is_one_edit_away = [&token](const std::string& dictionary_word) -> bool {
      return levenshtein(token.content, dictionary_word) == 1;
    };

    auto suggestion_view = dictionary | rv::filter(is_one_edit_away);

    std::set<std::string> suggestion_token(suggestion_view.begin(), suggestion_view.end());

    return Misspelling{token, suggestion_token};
  };

  auto has_suggestions = [](const Misspelling& misspelling) -> bool {
    return !misspelling.suggestions.empty();
  };

  auto misspelling_result = source 
                          | rv::filter(is_misspelled)
                          | rv::transform(to_misspelling)
                          | rv::filter(has_suggestions);

  return std::set<Misspelling>(misspelling_result.begin(), misspelling_result.end());
};

/* Helper methods */

#include "utils.cpp"