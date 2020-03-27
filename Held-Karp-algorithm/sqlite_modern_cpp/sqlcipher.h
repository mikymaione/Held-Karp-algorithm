/*
MIT License

Copyright (c) 2017 aminroosta

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#ifndef SQLITE_HAS_CODEC
#define SQLITE_HAS_CODEC
#endif

#include "../sqlite_modern_cpp.h"

namespace sqlite {
	struct sqlcipher_config : public sqlite_config {
		std::string key;
	};

	class sqlcipher_database : public database {
	public:
		sqlcipher_database(std::string db, const sqlcipher_config &config) : database(db, config) {
			set_key(config.key);
		}

		sqlcipher_database(std::u16string db, const sqlcipher_config &config) : database(db, config) {
			set_key(config.key);
		}

		void set_key(const std::string &key) {
			if (auto ret = sqlite3_key(_db.get(), key.data(), key.size()))
				errors::throw_sqlite_error(ret);
		}

		void set_key(const std::string &key, const std::string &db_name) {
			if (auto ret = sqlite3_key_v2(_db.get(), db_name.c_str(), key.data(), key.size()))
				errors::throw_sqlite_error(ret);
		}

		void rekey(const std::string &new_key) {
			if (auto ret = sqlite3_rekey(_db.get(), new_key.data(), new_key.size()))
				errors::throw_sqlite_error(ret);
		}

		void rekey(const std::string &new_key, const std::string &db_name) {
			if (auto ret = sqlite3_rekey_v2(_db.get(), db_name.c_str(), new_key.data(), new_key.size()))
				errors::throw_sqlite_error(ret);
		}
	};
}
