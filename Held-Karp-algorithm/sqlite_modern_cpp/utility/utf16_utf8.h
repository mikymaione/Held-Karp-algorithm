/*
MIT License

Copyright (c) 2017 aminroosta

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <locale>
#include <string>
#include <algorithm>

#include "../errors.h"

namespace sqlite {
	namespace utility {
		inline std::string utf16_to_utf8(const std::u16string &input) {
			struct : std::codecvt<char16_t, char, std::mbstate_t> {
			} codecvt;
			std::mbstate_t state{};
			std::string result((std::max)(input.size() * 3 / 2, std::size_t(4)), '\0');
			const char16_t *remaining_input = input.data();
			std::size_t produced_output = 0;
			while (true) {
				char *used_output;
				switch (codecvt.out(state, remaining_input, &input[input.size()],
					remaining_input, &result[produced_output],
					&result[result.size() - 1] + 1, used_output)) {
				case std::codecvt_base::ok:
					result.resize(used_output - result.data());
					return result;
				case std::codecvt_base::noconv:
					// This should be unreachable
				case std::codecvt_base::error:
					throw errors::invalid_utf16("Invalid UTF-16 input", "");
				case std::codecvt_base::partial:
					if (used_output == result.data() + produced_output)
						throw errors::invalid_utf16("Unexpected end of input", "");
					produced_output = used_output - result.data();
					result.resize(
						result.size()
						+ (std::max)((&input[input.size()] - remaining_input) * 3 / 2,
							std::ptrdiff_t(4)));
				}
			}
		}
	} // namespace utility
} // namespace sqlite
