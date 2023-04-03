/**
 * Copyright (C) 2023 Gil Barbosa Reis.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the “Software”), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "godot_utils.hpp"

#include <godot_cpp/godot.hpp>

using namespace godot;

namespace luagdextension {

std::string to_std_string(const String& s) {
	return to_std_string(s.utf8());
}

std::string to_std_string(const CharString& s) {
	return std::string(s.get_data(), s.length());
}

std::string_view to_string_view(const CharString& s) {
	return std::string_view(s.get_data(), s.length());
}

std::string_view to_string_view(const PackedByteArray& bytes) {
	return std::string_view((const char *) bytes.ptr(), bytes.size());
}

String error_to_string(Error error) {
	switch (error) {
		case OK: return "Ok";
		case FAILED: return "Failed";
		case ERR_UNAVAILABLE: return "Unavailable";
		case ERR_UNCONFIGURED: return "Unconfigured";
		case ERR_UNAUTHORIZED: return "Unauthorized";
		case ERR_PARAMETER_RANGE_ERROR: return "Parameter Range Error";
		case ERR_OUT_OF_MEMORY: return "Out Of Memory";
		case ERR_FILE_NOT_FOUND: return "File Not Found";
		case ERR_FILE_BAD_DRIVE: return "File Bad Drive";
		case ERR_FILE_BAD_PATH: return "File Bad Path";
		case ERR_FILE_NO_PERMISSION: return "File No Permission";
		case ERR_FILE_ALREADY_IN_USE: return "File Already In Use";
		case ERR_FILE_CANT_OPEN: return "File Can't Open";
		case ERR_FILE_CANT_WRITE: return "File Can't Write";
		case ERR_FILE_CANT_READ: return "File Can't Read";
		case ERR_FILE_UNRECOGNIZED: return "File Unrecognized";
		case ERR_FILE_CORRUPT: return "File Corrupt";
		case ERR_FILE_MISSING_DEPENDENCIES: return "File Missing Dependencies";
		case ERR_FILE_EOF: return "File EOF";
		case ERR_CANT_OPEN: return "Can't Open";
		case ERR_CANT_CREATE: return "Can't Create";
		case ERR_QUERY_FAILED: return "Query Failed";
		case ERR_ALREADY_IN_USE: return "Already In Use";
		case ERR_LOCKED: return "Locked";
		case ERR_TIMEOUT: return "Timeout";
		case ERR_CANT_CONNECT: return "Can't Connect";
		case ERR_CANT_RESOLVE: return "Can't Resolve";
		case ERR_CONNECTION_ERROR: return "Connection Error";
		case ERR_CANT_ACQUIRE_RESOURCE: return "Can't Acquire Resource";
		case ERR_CANT_FORK: return "Can't Fork";
		case ERR_INVALID_DATA: return "Invalid Data";
		case ERR_INVALID_PARAMETER: return "Invalid Parameter";
		case ERR_ALREADY_EXISTS: return "Already Exists";
		case ERR_DOES_NOT_EXIST: return "Does Not Exist";
		case ERR_DATABASE_CANT_READ: return "Database Cant Read";
		case ERR_DATABASE_CANT_WRITE: return "Database Cant Write";
		case ERR_COMPILATION_FAILED: return "Compilation Failed";
		case ERR_METHOD_NOT_FOUND: return "Method Not Found";
		case ERR_LINK_FAILED: return "Link Failed";
		case ERR_SCRIPT_FAILED: return "Script Failed";
		case ERR_CYCLIC_LINK: return "Cyclic Link";
		case ERR_INVALID_DECLARATION: return "Invalid Declaration";
		case ERR_DUPLICATE_SYMBOL: return "Duplicate Symbol";
		case ERR_PARSE_ERROR: return "Parse Error";
		case ERR_BUSY: return "Busy";
		case ERR_SKIP: return "Skip";
		case ERR_HELP: return "Help";
		case ERR_BUG: return "Bug";
		case ERR_PRINTER_ON_FIRE: return "Printer On Fire";
	}
	return "?";
}


}

