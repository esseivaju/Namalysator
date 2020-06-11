#ifndef BLOCKCUTTER_H_
#define BLOCKCUTTER_H_

#include <string>
#include <set>
#include <map>
#include <vector>
#include <assert.h>
#include "altorect.h"
#include "tiffio.h"

#include <QImage>
#include <QString>

extern "C" {
#include "jpeglib.h"
}

#define LANCZOS_RADIUS	5

class jpg_writer
{
	jpeg_compress_struct	cinfo;
	jpeg_error_mgr			jerr;
	FILE 					*fpout;
	int 					line;
	int						height;
public:
	// public so it's writable
	JSAMPLE					*jpgbuf;
	jpg_writer() : fpout(0), line(0), jpgbuf(0) {
		memset(&cinfo, 0, sizeof(cinfo));
		memset(&jerr, 0, sizeof(jerr));
	}
	~jpg_writer()
	{
		delete [] jpgbuf;
		if (fpout) {
			fclose(fpout);
		}
	}
	int open_jpg(const char *out_name, bool is_color, const alto_rectangle &r, int quality, int dpi = 300);
	int open_jpg_mem(std::vector<char> *out, bool is_color, const alto_rectangle &r, int quality, int dpi = 300);
	void write_scanline();
	void close();
	bool done() const;
};

class block_cutter
{
	struct block_info
	{
		std::string		fname;
		jpg_writer		writer;
	};
		
	typedef std::map<alto_rectangle, block_info, altorect_sort> cut_t;

	bool			m_little_endian;
	bool			m_is_color;
	bool			m_black_is_zero;
	TIFF 			*m_tif;
	QImage			*m_jp2;
	bool			is_uncompressed_tiff;
	alto_rectangle	m_orig;
	unsigned char	*m_buf[LANCZOS_RADIUS];
	unsigned int	m_scanlinesize;
	unsigned int	m_bitsperpixel;
	cut_t			to_cut;
	int				m_quality;

	void debug_block(cut_t::iterator it);
	void scan_line(unsigned char* buf, int line);
	int open_tiff(const char *in_name);
	int open_jp2(const char* in_name);
	void rescale_buf(tdata_t buf, tdata_t outbuf, uint32 w, int factor);
	void extend_blocks();
	void clean_up();
	
public:
	block_cutter();
	~block_cutter();
	void add(const alto_rectangle &ar, const std::string &fname);
	void cut_all(const std::string &inputfile, float factor = 1.0, std::map<std::string, std::vector<char> > *out_bytes = 0);
	void convert_file(const std::string &inputfile, const std::string &outputfile, float factor = 1.0, int quality = 75, bool color = false);
};

enum file_formats
{
	format_unreadable,
	format_unknown,
	format_tiff,
	format_jpeg,
	format_png,
	format_gif
};

class image_utils
{
public:
	file_formats identify_format(const std::string &fname);
	bool is_tiff(const std::string &fname);
	size_t count_colours(const std::string &fname, int abs_difference = 20);
	bool get_dimensions(const std::string &fname, int &width, int &height, int &xdpi, int &ydpi, file_formats &format);
};

class ThumbCreator
{
	int				iw, ih, tw, th;
	int				max_height;
	unsigned char **m_buf;
	unsigned int	m_scanlinesize;
	unsigned int	m_bitsperpixel;
	int				buf_y_start;
	int				buf_y_end;
	TIFF 			*m_tif;
	jpg_writer		writer;
	bool			m_little_endian;
	bool			m_is_color;

	int open_tiff(const char *in_name);
	void init_buf(int maxwidth, int maxheight);
	void read_tiff(int starty, int endy);
	unsigned char GetPixelGreyBE(int x, int y) {
		if (x < 0) { return m_buf[y - buf_y_start][0]; }
		if (x >= iw) { return m_buf[y - buf_y_start][iw - 1]; }
		assert(y >= buf_y_start && y < buf_y_end);
		return m_buf[y - buf_y_start][x]; 
	}
	unsigned char GetPixelGreyLE(int x, int y) { 
		if (x < 0) { return m_buf[y - buf_y_start][0]; }
		if (x >= iw) { return m_buf[y - buf_y_start][iw - 1]; }
		assert(y >= buf_y_start && y < buf_y_end);
		return (255 - m_buf[y - buf_y_start][x]); 
	}
	RGBQUAD GetPixelRGB(int x, int y) { 
		if (x < 0) { x = 0; }
		if (x >= iw) { x = iw - 1; }
		RGBQUAD r;
		r.rgbRed = m_buf[y - buf_y_start][x * 3];
		r.rgbGreen = m_buf[y - buf_y_start][x * 3 + 1];
		r.rgbBlue = m_buf[y - buf_y_start][x * 3 + 2];
		return r;
	}
	void SetPixelRGB(int x, const RGBQUAD &r) {
		writer.jpgbuf[x * 3] = r.rgbRed;
		writer.jpgbuf[x * 3 + 1] = r.rgbGreen;
		writer.jpgbuf[x * 3 + 2] = r.rgbBlue;
	}
	void SetPixelGrey(int x, unsigned int c) {
		writer.jpgbuf[x] = c;
	}
public:
	ThumbCreator() : iw(0), ih(0), tw(0), th(0), max_height(0), m_buf(0) {}
	bool CreateThumb(const std::string &inputfile, const std::string &outputfile, int maxwidth, int maxheight, int quality);
	~ThumbCreator();
};


#endif // BLOCKCUTTER_H_
