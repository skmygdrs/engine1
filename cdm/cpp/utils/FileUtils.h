/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once

#include "CommonDataModel.h"

#if defined(_MSC_VER) || defined(__MINGW64_VERSION_MAJOR)

#include <windows.h>

#define MAXPATH MAX_PATH
#define GETCWD _getcwd
#define MKDIR(x) mkdir(x)

#else

#include <unistd.h>

#if defined(__APPLE__) && defined(__clang__)
#include <sys/syslimits.h>
#endif

#define MAXPATH PATH_MAX
#define GETCWD getcwd
#define MKDIR(x) mkdir(x, 0755)

#if defined(__gnu_linux__)
#include <cstring>
#endif

#endif

bool CDM_DECL CreateFilePath(const std::string&);
std::string CDM_DECL Replace(const std::string& original, const std::string& replace, const std::string& withThis);
void CDM_DECL ListFiles(const std::string& dir, std::vector<std::string>& files, const std::string& mask = "");
std::string CDM_DECL GetCurrentWorkingDirectory();
void CDM_DECL DeleteDirectory(const std::string &dir, bool bDeleteSubdirectories=true);
bool CDM_DECL IsDirectory(struct dirent* ent);
