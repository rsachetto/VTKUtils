/*
 * file_util.cpp
 *
 *  Created on: 26/05/2011
 *      Author: sachetto
 */

#include "file_util.h"
#include <cstdlib>
#include <cstdio>
#include <dirent.h>
#include <errno.h>
#include <algorithm>
#include <sys/stat.h>
#include <fcntl.h>

bool startsWith(std::string &str, std::string &prefix) {

	if(str.length() < prefix.length()) return false;
	bool ans = true;

	for(unsigned int i = 0; i < prefix.length(); i++) {
		ans &= (str[i] == prefix[i]);
	}

	return ans;
}

int getFilesFromDir (std::string dir, std::vector<std::string> &files, std::string filter)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
    	std::string file_name(dirp->d_name);
    	if(!filter.empty()) {
    		if(startsWith(file_name, filter)) {
    	        files.push_back(dir +"/" + std::string(dirp->d_name));
    		}
    	}
    	else {
            files.push_back(dir +"/" + std::string(dirp->d_name));
    	}
    }

    std::sort(files.begin(), files.end());

    closedir(dp);
    return 0;
}

int createDir(const char* dir_name) {
	return mkdir(dir_name, 0755);
}
