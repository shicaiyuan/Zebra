/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Swan
 *    > Created Time:  2016-12-06 09:53:07
**/

#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <fstream>
#include <vector>
#include <memory>
#include <string>

#include "object.hpp"
#include "light.hpp"
#include "parameter.hpp"

namespace Swan {

class Parser
{
	public:

		Parser(const char *file):line_(1) {
			std::ifstream in(file);
			if (!in.is_open()) {
				std::cerr << "file " << file << " not existed :(\n";
				exit(-1);
			}
			std::string str;
			for (; !in.eof() || str.size(); ++line_, str.clear(), std::getline(in, str)) {
				if (!str.size() || str.find("#") != std::string::npos)
					continue;
				parameter_ = Parameter(str, line_);
				std::string s(parameter_.FindString());
				if (s == "Object") objects_.push_back(std::make_shared<Object>(FindObject()));
				else if (s == "Light") lights_.push_back(std::make_shared<Light>(FindLight()));
				else {
					std::cerr << "line: " << line_ << ": syntax error :(\n";
					exit(-1);
				}
			}
		}

		Object* FindObject() {
			std::string s(parameter_.FindString());
			const BSDF *bsdf = nullptr;
			if (s == "Diffuse") bsdf = NewDiffuseDSDF(parameter_);
			else if (s == "Reflect") bsdf = NewReflectBSDF(parameter_);
			else {
				std::cerr << "line: " << line_ << ": syntax error :(\n";
				exit(-1);
			}
			std::string o(parameter_.FindString());
			assert(o == "Sphere");
			return NewSphere(parameter_, bsdf);
		}

		Light* FindLight() {
			std::string s(parameter_.FindString());
			if (s == "Point") return NewPointLight(parameter_);
			else if (s == "Directional") return NewDirectionalLight(parameter_);
			std::cerr << "line: " << line_ << ": syntax error :(\n";
			exit(-1);
		}

	private:
		int       line_;
		Parameter parameter_;
		std::vector<std::shared_ptr<Light>>  lights_;
		std::vector<std::shared_ptr<Object>> objects_;
};

} // namespace Swan

#endif /* _PARSER_HPP_ */