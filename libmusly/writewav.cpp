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

#include "writewav.h"

namespace musly {

writewav::writewav() {
}

template <typename Word>
std::ostream& write( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
{
    for (; size; --size, value >>= 8)
    outs.put( static_cast <char> (value & 0xFF) );
    return outs;
}

void writewav::writewavdata(const char* outFile, std::vector<float> buf, int bufSize, int sampleRate, short NumChannels, short BitsPerSample) {
    std::ofstream stream(outFile, std::ios::binary);                // Open file stream at "outFile" location
    //bufSize = int2size_t(bufSize);

    /* The "RIFF" chunk descriptor */
    //stream.write("RIFF", 4);                                    // ChunkID
    //write(stream, 36 + bufSize, 4);                             // ChunkSize
    //stream.write("WAVE", 4);                                    // Format

    /* The "fmt" sub-chunk */
    stream << "RIFF----WAVEfmt ";;                                // Subchunk1ID
    write(stream, 16, 4);                                         // Subchunk1Size
    write(stream, 3, 2);                                          // AudioFormat (1 = PCM, 3 = WAV)
    write(stream, NumChannels, 2);                                // NumChannels
    write(stream, sampleRate, 4);                                 // SampleRate
    write(stream, sampleRate * NumChannels * sizeof(float), 4);   // ByteRate == SampleRate * NumChannels * BitsPerSample/8
    write(stream, NumChannels * sizeof(float), 2);                // FrameSize / BlockAlign == NumChannels * BitsPerSample/8
    write(stream, 8*sizeof(float), 2);                            // BitsPersample

    // Write the data chunk header
    size_t data_chunk_pos = stream.tellp();
    stream << "data----";  // (chunk size to be filled in later)

    /* The "data" sub-chunk */
    //stream.write("data", 4);                                    // Subchunk2ID
    //stream.write((const char*)&bufSize, 4);                     // Subchunk2Size (= bufferSize) == NumSamples * NumChannels * BitsPerSample/8
    for(int i = 0; i < bufSize; ++i) {                            // data
        stream.write( reinterpret_cast<const char*> (&buf[i]), sizeof( float ));                     
    }

    // (We'll need the final file size to fix the chunk sizes above)
    size_t file_length = stream.tellp();

    // Fix the data chunk header to contain the data size
    stream.seekp( data_chunk_pos + 4 );
    write( stream, file_length - data_chunk_pos + 8 );

    // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
    stream.seekp( 0 + 4 );
    write( stream, file_length - 8, 4 );
}

} /* namespace musly */

/*
http://www.cplusplus.com/forum/beginner/166954/

#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

namespace little_endian_io
{
  template <typename Word>
  std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
  {
    for (; size; --size, value >>= 8)
      outs.put( static_cast <char> (value & 0xFF) );
    return outs;
  }
}
using namespace little_endian_io;

int main()
{
  ofstream f( "example.wav", ios::binary );

  // Write the file headers
  f << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
  write_word( f,     16, 4 );  // no extension data
  write_word( f,      1, 2 );  // PCM - integer samples
  write_word( f,      2, 2 );  // two channels (stereo file)
  write_word( f,  44100, 4 );  // samples per second (Hz)
  write_word( f, 176400, 4 );  // (Sample Rate * BitsPerSample * Channels) / 8
  write_word( f,      4, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)
  write_word( f,     16, 2 );  // number of bits per sample (use a multiple of 8)

  // Write the data chunk header
  size_t data_chunk_pos = f.tellp();
  f << "data----";  // (chunk size to be filled in later)
  
  // Write the audio samples
  // (We'll generate a single C4 note with a sine wave, fading from left to right)
  constexpr double two_pi = 6.283185307179586476925286766559;
  constexpr double max_amplitude = 32760;  // "volume"

  double hz        = 44100;    // samples per second
  double frequency = 261.626;  // middle C
  double seconds   = 2.5;      // time

  int N = hz * seconds;  // total number of samples
  for (int n = 0; n < N; n++)
  {
    double amplitude = (double)n / N * max_amplitude;
    double value     = sin( (two_pi * n * frequency) / hz );
    write_word( f, (int)(                 amplitude  * value), 2 );
    write_word( f, (int)((max_amplitude - amplitude) * value), 2 );
  }
  
  // (We'll need the final file size to fix the chunk sizes above)
  size_t file_length = f.tellp();

  // Fix the data chunk header to contain the data size
  f.seekp( data_chunk_pos + 4 );
  write_word( f, file_length - data_chunk_pos + 8 );

  // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
  f.seekp( 0 + 4 );
  write_word( f, file_length - 8, 4 ); 
}
*/

// https://github.com/aazpitarte/sound-maker/blob/master/sound_maker.cpp
// https://stackoverflow.com/questions/22226872/two-problems-when-writing-to-wav-c