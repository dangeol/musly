/**
 * This source file: added by Daniel Oellers for testing purposes
 * Implementation according to http://soundfile.sapp.org/doc/WaveFormat/ 
 * Code partially from http://www.cplusplus.com/forum/beginner/166954/
 * 
 * Copyright 2013-2014, Dominik Schnitzer <dominik@schnitzer.at>
 *
 * This file is part of Musly, a program for high performance music
 * similarity computation: http://www.musly.org/.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef MUSLY_WRITEWAV_H_
#define MUSLY_WRITEWAV_H_

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

namespace musly {
    namespace little_endian_io
    {
        template <typename Word>
        std::ostream& write( std::ostream& outs, Word value, unsigned size = sizeof( Word ) );
    }

class writewav {
private:

public:
    writewav();
    void writewavdata(const char* outFile, std::vector<float> buf, int bufSize, int sampleRate, short NumChannels, short BitsPerSample);
};

} /* namespace musly */
#endif /* MUSLY_WRITEWAV_H_ */
