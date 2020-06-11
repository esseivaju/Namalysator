#include <list>
#include <limits.h>
#include "blockcutter.h"
#include <vector>

// Memory destination for JPEG writing
const int INITIAL_SIZE = 1024; // 1KB buffer
// Precision for the ThumbCreator
#define INT_SHIFT 4
#define INT_PRECISION (1 << INT_SHIFT)
#define INT_MASK (INT_PRECISION - 1)

struct vector_dest : jpeg_destination_mgr
{
	std::vector<char> *data;
};

METHODDEF(void) init_vecdest(j_compress_ptr cinfo)
{
	vector_dest *dest = (vector_dest *) cinfo->dest;
	dest->data->resize(INITIAL_SIZE);
	dest->next_output_byte = (JOCTET *) (&(dest->data->operator[](0)));
	dest->free_in_buffer = INITIAL_SIZE;
}

METHODDEF(boolean) empty_vecdest(j_compress_ptr cinfo)
{
	vector_dest *dest = (vector_dest *) cinfo->dest;
	// just make the buffer bigger by doubling the size
	size_t currsize = dest->data->size();
	dest->data->resize(currsize * 2);
	dest->next_output_byte = (JOCTET *) (&dest->data->operator[](currsize));
	dest->free_in_buffer = currsize;
	return TRUE;
}

METHODDEF(void) term_vecdest(j_compress_ptr cinfo)
{
	vector_dest *dest = (vector_dest *) cinfo->dest;
	// cut the buffer off where we ended
	size_t data_used = dest->data->size() - dest->free_in_buffer;
	dest->data->resize(data_used);
}

void jpeg_vectdest(j_compress_ptr cinfo, std::vector<char> *out)
{
	vector_dest *d = 0; 

	if (cinfo->dest == 0) {
		d = (vector_dest *) (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(vector_dest));
		memset(d, 0, sizeof(vector_dest));
		cinfo->dest = d;
	} else {
		d = (vector_dest *) cinfo->dest;
	}
	d->data = out;
	d->empty_output_buffer = empty_vecdest;
	d->init_destination = init_vecdest;
	d->term_destination = term_vecdest;
}


// JPG Writer

int jpg_writer::open_jpg(const char *out_name, bool is_color, const alto_rectangle &r, int quality, int dpi)
{
	fpout = fopen(out_name, "wb");
	if (!fpout) {
		return -2;
	}
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, fpout);
	cinfo.image_width = r.width; 	/* image width and height, in pixels */
	cinfo.image_height = r.height;
	if (is_color) {
		cinfo.input_components = 3;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	} else {
		cinfo.input_components = 1;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_GRAYSCALE; 	/* colorspace of input image */
	}
	jpeg_set_defaults(&cinfo);
	cinfo.X_density = dpi;
	cinfo.Y_density = dpi;
	cinfo.density_unit = 1;
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
	jpeg_start_compress(&cinfo, TRUE);
	jpgbuf = new JSAMPLE[cinfo.input_components * cinfo.image_width];
	line = 0;
	height = r.height;
	return 0;
}

int jpg_writer::open_jpg_mem(std::vector<char> *out, bool is_color, const alto_rectangle &r, int quality, int dpi)
{
	memset(&cinfo, 0, sizeof(cinfo));
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_vectdest(&cinfo, out);
	cinfo.image_width = r.width; 	/* image width and height, in pixels */
	cinfo.image_height = r.height;
	if (is_color) {
		cinfo.input_components = 3;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	} else {
		cinfo.input_components = 1;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_GRAYSCALE; 	/* colorspace of input image */
	}
	jpeg_set_defaults(&cinfo);
	cinfo.X_density = dpi;
	cinfo.Y_density = dpi;
	cinfo.density_unit = 1;
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
	jpeg_start_compress(&cinfo, TRUE);
	jpgbuf = new JSAMPLE[cinfo.input_components * cinfo.image_width];
	line = 0;
	height = r.height;
	return 0;
}

void jpg_writer::write_scanline()
{
	jpeg_write_scanlines(&cinfo, &jpgbuf, 1);
	++line;
}

bool jpg_writer::done() const
{
	return line >= height;
}

void jpg_writer::close()
{
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	if (fpout) {
		fclose(fpout);
	}
	delete [] jpgbuf;
	jpgbuf = 0;
}

// TIFF MAGIC NUMBERS
// If it's liitle endian, we have to use 255 - color to get the correct color
const unsigned char TIFF_MAGIC_NUMBER_BE[] = {'I', 'I', 42, 0};
const unsigned char TIFF_MAGIC_NUMBER_LE[] = {'M', 'M', 0, 42};
const unsigned char JPEG_MAGIC_NUMBER[] = {0xFF, 0xD8, 0xFF, 0xE0};
const unsigned char PNG_MAGIC_NUMBER[] = {0x89, 0x50, 0x4E, 0x47};
const unsigned char GIF_MAGIC_NUMBER[] = {0x47, 0x49, 0x46, 0x38};


block_cutter::block_cutter()
{
	m_tif = 0;
	for (int i = 0; i < LANCZOS_RADIUS; ++i) {
		m_buf[i] = 0;
	}
	m_scanlinesize = 0;
	m_bitsperpixel = 0;
	m_little_endian = false;
	is_uncompressed_tiff = false;
	m_quality = 75;
}

block_cutter::~block_cutter()
{
	clean_up();
}

void block_cutter::clean_up()
{
	for (int i = 0; i < LANCZOS_RADIUS; ++i) {
		delete [] m_buf[i];
		m_buf[i] = 0;
	}
	if (m_tif) {
		TIFFClose(m_tif);
	}
	m_tif = 0;
	m_bitsperpixel = 0;	

	if (m_jp2) {
		delete m_jp2;
		m_jp2 = 0;
	}
}

// Image functions

static const unsigned char BIT_MASKS[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

void block_cutter::rescale_buf(tdata_t buf, tdata_t outbuf, uint32 w, int factor)
// Scales to 8 bits per pixel greyscale
{
	unsigned char *ucbuf = (unsigned char *) buf;
	unsigned char *ucoutbuf = (unsigned char *) outbuf;
	uint32 neww = w / factor;
	uint32 o = 0, i;
	if (m_is_color) {
		neww *= 3;
		if (m_bitsperpixel == 8) {
			if (m_little_endian) {
				for (i = 0; i < neww; i += 3, o += factor) {
					ucoutbuf[i] = 255 - ucbuf[o];
					ucoutbuf[i + 1] = 255 - ucbuf[o];
					ucoutbuf[i + 2] = 255 - ucbuf[o];
				}
			} else {
				for (i = 0; i < neww; i += 3, o += factor) {
					ucoutbuf[i] = ucbuf[o];
					ucoutbuf[i + 1] = ucbuf[o];
					ucoutbuf[i + 2] = ucbuf[o];
				}
			}
		} else if (m_bitsperpixel == 24) {
			int fac3 = factor * 3;
			for (i = 0; i < neww; i += 3, o += fac3) {
				ucoutbuf[i] = ucbuf[o];
				ucoutbuf[i + 1] = ucbuf[o + 1];
				ucoutbuf[i + 2] = ucbuf[o + 2];
			}
		}
	} else {
		if (m_bitsperpixel == 1) {
			if (m_black_is_zero) {
				for (i = 0; i < neww; ++i, o += factor) {
					ucoutbuf[i] = (ucbuf[o / 8] & BIT_MASKS[o % 8] ? 0xFF : 0);
				}
			} else {
				for (i = 0; i < neww; ++i, o += factor) {
					if ((ucbuf[o / 8] & BIT_MASKS[o % 8]) > 0) {
						ucoutbuf[i] = 0;
					} else {
						ucoutbuf[i] = 0xFF;
					}
				}
			}
		} else if (m_bitsperpixel == 8) {
			if (m_little_endian) {
				for (i = 0; i < neww; ++i, o += factor) {
					ucoutbuf[i] = 255 - ucbuf[o];
				}
			} else {
				for (i = 0; i < neww; ++i, o += factor) {
					ucoutbuf[i] = ucbuf[o];
				}
			}
		} else if (m_bitsperpixel == 16) {
			int fac2 = factor * 2;
			for (i = 0; i < neww; ++i, o += fac2) {
				int val = ucbuf[o];
				val += ucbuf[o + 1];
				ucoutbuf[i] = val / 2;
			}
		} else if (m_bitsperpixel == 24) {
			int fac3 = factor * 3;
			for (i = 0; i < neww; ++i, o += fac3) {
				int val = ucbuf[o];
				val += ucbuf[o + 1];
				val += ucbuf[o + 2];
				ucoutbuf[i] = val / 3;
			}	
		} else if (m_bitsperpixel == 32) {
			int fac4 = factor * 4;
			for (i = 0; i < neww; ++i, o += fac4) {
				int val = ucbuf[o];
				val += ucbuf[o + 1];
				val += ucbuf[o + 2];
				val += ucbuf[o + 3];
				ucoutbuf[i] = val / 4;
			}
		}
	}
}

int block_cutter::open_tiff(const char *in_name)
{
	FILE *fp = fopen(in_name, "rb");
	if (!fp) {
		return -1;
	}
	// Check whether it's big or little endian
	{
		char tmp[4];
		fread(tmp, 1, 4, fp);
		if (memcmp(tmp, TIFF_MAGIC_NUMBER_LE, 4) == 0) {
			m_little_endian = true;
		}
		fclose(fp);
	}
	
    m_tif = TIFFOpen(in_name, "r");
    if (!m_tif) {
		return -1;
	}
	uint16 compression = 0, photometric = 0; //  n_samples = 0
	uint32 tifheight, tifwidth;

	TIFFGetField(m_tif, TIFFTAG_IMAGELENGTH, &tifheight);
	m_orig.height = tifheight;
	TIFFGetField(m_tif, TIFFTAG_IMAGEWIDTH, &tifwidth);
	m_orig.width = tifwidth;
	TIFFGetField(m_tif, TIFFTAG_COMPRESSION, &compression);
	if (compression == 1) {
		is_uncompressed_tiff = true;
	} else {
		is_uncompressed_tiff = false;
	}
	TIFFGetField(m_tif, TIFFTAG_PHOTOMETRIC, &photometric);
	if (photometric == 0) {
		m_black_is_zero = false;
	} else {
		m_black_is_zero = true;
	}
	m_scanlinesize = TIFFScanlineSize(m_tif);
	m_bitsperpixel = m_scanlinesize * 8 / tifwidth;
	if (m_bitsperpixel == 24 || m_bitsperpixel == 32) {
		m_is_color = true;
	} else {
		m_is_color = false;
	}
	return 0;
}

int block_cutter::open_jp2(const char* in_name) {
	QString str(in_name);
	
	m_jp2 = new QImage(str);
	if (m_jp2->isNull())
		return 1;
	m_orig.height = m_jp2->height();
	m_orig.width = m_jp2->width();
	m_black_is_zero = true;
	m_scanlinesize = m_jp2->bytesPerLine();
	m_bitsperpixel = m_jp2->depth();
	m_is_color = (m_bitsperpixel == 24 || m_bitsperpixel == 32);
	return 0;
}

void block_cutter::scan_line(unsigned char* buf, int line) {
	if (m_tif) {
		TIFFReadScanline(m_tif, buf, line, 0);
	}
	else if (m_jp2) {
		unsigned char* line_start = m_jp2->scanLine(line);
		for (int i = 0; i < m_scanlinesize; ++i)
			buf[i] = line_start[i];
	}
}

void block_cutter::add(const alto_rectangle &ar, const std::string &fname)
{
	block_info bi;
	bi.fname = fname;
	if (to_cut.find(ar) != to_cut.end()) {
		fprintf(stderr, "BC: ERROR adding twice the same block\n");
	}
	to_cut[ar] = bi;
}

void block_cutter::debug_block(cut_t::iterator it)
{
	// printf("%20s  (%d,%d - %d,%d)\n", it->second.fname.c_str(), it->first.x, it->first.y, it->first.width, it->first.height);
}

// If a block has -1 as width or -1 as height, its width resp. height extends to the bottom of the file
void block_cutter::cut_all(const std::string &inputfile, float factor, std::map<std::string, std::vector<char> > *out_bytes)
{
 	if (to_cut.begin() == to_cut.end()) {
		// Nothing to do
		return;
	}
	if (factor == 0.0) {
		fprintf(stderr, "BC: Invalid scaling factor\n");
	}
	// printf("BC: Open %s\n", inputfile.c_str());
	std::string file_ext = inputfile.substr(inputfile.length() - 3, 3);
	if (file_ext == "tif" && open_tiff(inputfile.c_str()) != 0) {
		fprintf(stderr, "unable to open file\n");
		return;
	}
	else if (file_ext == "jp2" && open_jp2(inputfile.c_str())) {
		return;
	}
	extend_blocks();
	m_buf[0] = new unsigned char[m_scanlinesize];
	int ifac = int(1.0/factor);

	// Start with the first block
	std::list<cut_t::iterator> active_blocks;
	cut_t::iterator next;
	next = to_cut.begin();
	int row = next->first.y;
	std::map<std::string, std::vector<char> >::iterator out_byte_it;
	std::pair<std::map<std::string, std::vector<char> >::iterator, bool> out_byte_pair;
	// Add all blocks starting at or before this line
	while (next != to_cut.end() && (next->first.y <= row)) {
		// printf("Block added: ");
		debug_block(next);
		active_blocks.push_back(next);
		alto_rectangle ar(next->first.x / ifac, next->first.y / ifac, next->first.width / ifac, next->first.height / ifac);
		ar.resize_to_minimum_1();
		if (out_bytes) {
			out_byte_pair = out_bytes->insert(std::make_pair(next->second.fname, std::vector<char>()));
			next->second.writer.open_jpg_mem(&out_byte_pair.first->second, m_is_color, ar, m_quality);
		} else {
			next->second.writer.open_jpg(next->second.fname.c_str(), m_is_color, ar, m_quality);
		}
		++next;
	}
	std::list<cut_t::iterator>::iterator lit;
	while (row < m_orig.height && (next != to_cut.end() || !active_blocks.empty())) {
		// printf("%d\n", row);
		scan_line(m_buf[0], row);
		for (lit = active_blocks.begin(); lit != active_blocks.end(); ++lit) {
			rescale_buf(m_buf[0] + (*lit)->first.x * m_bitsperpixel / 8, (*lit)->second.writer.jpgbuf, (*lit)->first.width, ifac);
			(*lit)->second.writer.write_scanline();
		}
		if (m_tif && !is_uncompressed_tiff) {
			// Read ifac - 1 rows and throw data away
			for (int i = 1; i < ifac; ++i) {
				scan_line(m_buf[0], row + i);
			}
		}
		row += ifac;
		// Throw out blocks that are finished
		lit = active_blocks.begin();
		while (lit != active_blocks.end()) {
			if ((*lit)->second.writer.done()) {
				// printf("Block done: ");
				debug_block(*lit);
				(*lit)->second.writer.close();
				lit = active_blocks.erase(lit);
			} else {
				++lit;
			}
		}
		// Add new blocks
		while (next != to_cut.end() && next->first.y <= row) {
			// printf("Block added: ");
			debug_block(next);
			active_blocks.push_back(next);
			alto_rectangle ar(next->first.x / ifac, next->first.y / ifac, next->first.width / ifac, next->first.height / ifac);
			ar.resize_to_minimum_1();
			if (out_bytes) {
				out_byte_pair = out_bytes->insert(std::make_pair(next->second.fname, std::vector<char>()));
				next->second.writer.open_jpg_mem(&out_byte_pair.first->second, m_is_color, ar, m_quality);
			} else {
				next->second.writer.open_jpg(next->second.fname.c_str(), m_is_color, ar, m_quality);
			}
			++next;
		}
		// If no block is active, skip until the next block
		if (next != to_cut.end() && active_blocks.empty()) {
			// printf("Skipping from %d to %d\n", row, next->first.y);
			row = next->first.y;
			// Add all blocks starting at or before this line
			while (next != to_cut.end() && (next->first.y <= row)) {
				// printf("Block added: ");
				debug_block(next);
				active_blocks.push_back(next);
				alto_rectangle ar(next->first.x / ifac, next->first.y / ifac, next->first.width / ifac, next->first.height / ifac);
				ar.resize_to_minimum_1();
				if (out_bytes) {
					out_byte_pair = out_bytes->insert(std::make_pair(next->second.fname, std::vector<char>()));
					next->second.writer.open_jpg_mem(&out_byte_pair.first->second, m_is_color, ar, m_quality);
				} else {
					next->second.writer.open_jpg(next->second.fname.c_str(), m_is_color, ar, m_quality);
				}
				++next;
			}			
		}
	}
	clean_up();
	// printf("BC: done\n");
}

void block_cutter::convert_file(const std::string &inputfile, const std::string &outputfile, float factor, int quality, bool color)
{
	if (open_tiff(inputfile.c_str()) != 0) {
		fprintf(stderr, "unable to open file %s\n", inputfile.c_str());
		return;
	}
	color = m_is_color;
	block_info block;
	m_buf[0] = new unsigned char[m_scanlinesize];
	int ifac = int(1.0/factor + 0.49);
	alto_rectangle ar(m_orig.x / ifac, m_orig.y / ifac, m_orig.width / ifac, m_orig.height / ifac);
	if (block.writer.open_jpg(outputfile.c_str(), color, ar, quality) != 0) {
		fprintf(stderr, "Unable to open file for writing %s\n", outputfile.c_str());
		clean_up();
		return;
	}
	for (long row = 0; row < m_orig.height && !block.writer.done(); row += ifac) {
		TIFFReadScanline(m_tif, m_buf[0], row, 0);
		rescale_buf(m_buf[0], block.writer.jpgbuf, m_orig.width, ifac);
		block.writer.write_scanline();
		if (!is_uncompressed_tiff) {
			// Read ifac - 1 rows and throw data away
			for (int i = 1; i < ifac; ++i) {
				TIFFReadScanline(m_tif, m_buf[0], row + i, 0);
			}
		}
	}
	block.writer.close();
	delete [] m_buf[0];
	m_buf[0] = 0;
	clean_up();
}

void block_cutter::extend_blocks()
{
	cut_t tmp;
	int w, h;
	for (cut_t::iterator it = to_cut.begin(); it != to_cut.end(); ++it) {
		w = it->first.width;
		h = it->first.height;
		if (w == -1) {
			w = m_orig.width - it->first.x;
		}
		if (h == -1) {
			h = m_orig.height - it->first.y;
		}
		tmp[alto_rectangle(it->first.x, it->first.y, w, h)] = it->second;
	}
	tmp.swap(to_cut);
}

file_formats image_utils::identify_format(const std::string &fname)
{
	FILE *fp = fopen(fname.c_str(), "rb");
	if (!fp) {
		return format_unreadable;
	}
	char tmp[4];
	int nread = fread(tmp, 1, 4, fp);
	fclose(fp);
	if (nread != 4) {
		return format_unreadable;
	}
	if (memcmp(tmp, TIFF_MAGIC_NUMBER_LE, 4) == 0) {
		return format_tiff;
	} else if (memcmp(tmp, TIFF_MAGIC_NUMBER_BE, 4) == 0) {
		return format_tiff;
	} else if (memcmp(tmp, JPEG_MAGIC_NUMBER, 4) == 0) {
		return format_jpeg;
	} else if (memcmp(tmp, PNG_MAGIC_NUMBER, 4) == 0) {
		return format_png;
	} else if (memcmp(tmp, GIF_MAGIC_NUMBER, 4) == 0) {
		return format_gif;
	} else {
		return format_unknown;
	}
}

bool image_utils::is_tiff(const std::string &fname)
{
	FILE *fp = fopen(fname.c_str(), "rb");
	if (!fp) {
		return false;
	}
	char tmp[4];
	int nread = fread(tmp, 1, 4, fp);
	fclose(fp);
	if (nread != 4) {
		return false;
	}
	if (memcmp(tmp, TIFF_MAGIC_NUMBER_LE, 4) == 0) {
		return true;
	} else if (memcmp(tmp, TIFF_MAGIC_NUMBER_BE, 4) == 0) {
		return true;
	} else {
		return false;
	}
}

bool image_utils::get_dimensions(const std::string &fname, int &width, int &height, int &xdpi, int &ydpi, file_formats &format)
{
	format = identify_format(fname);
	width = 0;
	height = 0;
	if (format == format_tiff) {
		TIFF *tif = TIFFOpen(fname.c_str(), "r");
		if (!tif) {
			return false;
		}
		uint32 w, h;
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
		width = w;
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
		height = h;
		float dpi;
		TIFFGetField(tif, TIFFTAG_XRESOLUTION, &dpi);
		xdpi = int(dpi);
		TIFFGetField(tif, TIFFTAG_YRESOLUTION, &dpi);
		ydpi = int(dpi);
		TIFFClose(tif);
		return true;
	} else if (format == format_jpeg) {
		FILE *fp = fopen(fname.c_str(), "rb");
		if (!fp) {
			return false;
		}
		jpeg_decompress_struct cinfo;
		jpeg_error_mgr jerr;
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		jpeg_stdio_src(&cinfo, fp);
		jpeg_read_header(&cinfo, TRUE);
		width = cinfo.image_width;
		height = cinfo.image_height;
		if (cinfo.density_unit == 0) { // unknown, this signals 72 dpi usually, but tell the caller
			xdpi = -1;
			ydpi = -1;
		} else if (cinfo.density_unit == 1) { // dpi
			xdpi = cinfo.X_density;
			ydpi = cinfo.Y_density;
		} else if (cinfo.density_unit == 2) { // dots per cm, convert to dpi
			xdpi = int(cinfo.X_density * 2.54);
			ydpi = int(cinfo.Y_density * 2.54);
		}
		jpeg_destroy_decompress(&cinfo);
		return true;
	} else {
		return false;
	}
}

// Helper functions

int max3(unsigned char *data)
{
	if (data[0] > data[1]) {
		if (data[0] > data[2]) {
			return 0;
		} else {
			return 2;
		}
	} else {
		if (data[1] > data[2]) {
			return 1;
		} else {
			return 2;
		}
	}
}

int min3(unsigned char *data)
{
	if (data[0] < data[1]) {
		if (data[0] < data[2]) {
			return 0;
		} else {
			return 2;
		}
	} else {
		if (data[1] < data[2]) {
			return 1;
		} else {
			return 2;
		}
	}
}

size_t image_utils::count_colours(const std::string &fname, int abs_difference)
{
	FILE *fp = fopen(fname.c_str(), "rb");
	if (!fp) {
		return 0;
	}
	bool m_little_endian = false;
	// Check whether it's big or little endian
	{
		char tmp[4];
		fread(tmp, 1, 4, fp);
		fclose(fp);
		if (memcmp(tmp, TIFF_MAGIC_NUMBER_LE, 4) == 0) {
			m_little_endian = true;
		} else if (!memcmp(tmp, TIFF_MAGIC_NUMBER_BE, 4) == 0) {
			return 0;
		}
	}
	
    TIFF *tif = TIFFOpen(fname.c_str(), "r");
    if (!tif) {
		return 0;
	}
	//uint16 n_samples = 0;
	uint32 tifheight, tifwidth;
	//bool image_ok = true;

	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &tifheight);
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &tifwidth);
	int scanlinesize = TIFFScanlineSize(tif);
	int bitsperpixel = scanlinesize / tifwidth * 8;
#ifdef GET_HISTOGRAM
	size_t counter[256];
	memset(counter, 0, 256 * sizeof(size_t));
#endif
	if (bitsperpixel != 24) {
		// Not a colour TIFF
		TIFFClose(tif);
		return 0;
	}
	size_t colours[3]; // RGB values found in colour image
	memset(colours, 0, 3 * sizeof(size_t));
	unsigned char *data = new unsigned char[scanlinesize];
	// Read the TIFF
	for (size_t row = 0; row < tifheight; ++row) {
		TIFFReadScanline(tif, data, row, 1);
		size_t o = 0;
		for (size_t i = 0; i < tifwidth; ++i, o+=3) {
			if (data[o] != data[o + 1] || data[o] != data[o + 2]) {
				// colour found
				int max = max3(data + o);
				int min = min3(data + o);
				if (data[max + o] - data[min + o] > abs_difference) {
					colours[max]++;
				}
#ifdef GET_HISTOGRAM
				counter[(data[max + o] + data[min + o]) / 2]++;
#endif
			} else {
#ifdef GET_HISTOGRAM
				counter[data[o]]++;
#endif
			}
		}			
	}
	TIFFClose(tif);
	delete [] data;
	return colours[0] + colours[1] + colours[2];
}

ThumbCreator::~ThumbCreator()
{
	if (m_buf) {
		for (int i = 0; i < max_height; ++i) {
			delete m_buf[i];
		}
		delete m_buf;
	}
}

int ThumbCreator::open_tiff(const char *in_name)
{
	FILE *fp = fopen(in_name, "rb");
	if (!fp) {
		return -1;
	}
	m_little_endian = false;
	// Check whether it's big or little endian
	{
		char tmp[4];
		fread(tmp, 1, 4, fp);
		if (memcmp(tmp, TIFF_MAGIC_NUMBER_LE, 4) == 0) {
			m_little_endian = true;
		}
		fclose(fp);
	}
	
	m_tif = TIFFOpen(in_name, "r");
	if (!m_tif) {
		return -1;
	}
	uint32 tifheight, tifwidth;

	TIFFGetField(m_tif, TIFFTAG_IMAGELENGTH, &tifheight);
	ih = tifheight;
	TIFFGetField(m_tif, TIFFTAG_IMAGEWIDTH, &tifwidth);
	iw = tifwidth;
	m_scanlinesize = TIFFScanlineSize(m_tif);
	m_bitsperpixel = m_scanlinesize / tifwidth * 8;
	if (m_bitsperpixel == 24 || m_bitsperpixel == 32) {
		m_is_color = true;
	} else {
		m_is_color = false;
	}
	return 0;
}

void ThumbCreator::init_buf(int maxwidth, int maxheight)
{
	if (maxheight == INT_MAX || maxheight == 0) {
		tw = maxwidth;
		th = maxwidth * ih / iw;
	} else {
		if (maxwidth > (maxheight * iw / ih)) {
			tw = maxheight * iw / ih;
			th = maxheight;
		} else {
			tw = maxwidth;
			th = maxwidth * ih / iw;
		}
	}
	max_height = (((ih << INT_SHIFT) / th) >> INT_SHIFT) + 4;
	m_buf = new unsigned char *[max_height];
	for (int i = 0; i < max_height; ++i) {
		m_buf[i] = new unsigned char[m_scanlinesize];
	}
	buf_y_start = 0;
}

void ThumbCreator::read_tiff(int starty, int endy)
{
	if (endy > ih) {
		endy = ih;
	}
	for (int i = starty; i < endy; ++i) {
		TIFFReadScanline(m_tif, m_buf[i - starty], i, 0);
	}
	buf_y_start = starty;
	buf_y_end = endy;
}

bool ThumbCreator::CreateThumb(const std::string &inputfile, const std::string &outputfile, int maxwidth, int maxheight, int quality)
{
	if (open_tiff(inputfile.c_str()) != 0) {
		return false;
	}
	init_buf(maxwidth, maxheight);
	int sx = tw;
	int sy = th;

	alto_rectangle r;
	r.x = r.y = 0;
	r.width = tw;
	r.height = th;
	if (writer.open_jpg(outputfile.c_str(), m_is_color, r, quality) != 0) {
		TIFFClose(m_tif);
		return false;
	}

	// resampling the image to thumbnail size

	// basically this is a discrete 2-dimensional  
	// integration using linear interpolation.
	// for performance reasons, all calculations are
	// done in integer arithmetics.

	// mediocre results for enlarging images, but
	// great results for reducing the size.

	int ry1, ry2, iy1, iy2, jy1, jy2;
	int rx1, rx2, ix1, ix2, jx1, jx2;

	for (int y=0; y<sy; y++) {
		ry1 = ((y * ih) << INT_SHIFT) / sy;
		ry2 = (((y+1) * ih) << INT_SHIFT) / sy;
		iy1 = ry1 >> INT_SHIFT;
		iy2 = (ry2 >> INT_SHIFT) + 1;
		jy1 = INT_MASK - (ry1 & INT_MASK);
		jy2 = ry2 & INT_MASK;
		if (iy2 >= ih) { iy2 = ih - 1; }
		read_tiff(iy1, iy2 + 1);
		for (int x=0; x<sx; x++) {
			rx1 = ((x * iw) << INT_SHIFT) / sx;
			rx2 = (((x+1) * iw) << INT_SHIFT) / sx;    
			ix1 = rx1 >> INT_SHIFT;
			ix2 = (rx2 >> INT_SHIFT) + 1;
			jx1 = INT_MASK - (rx1 & INT_MASK);
			jx2 = rx2 & INT_MASK;
			if (ix2 >= iw) { ix2 = iw - 1; }

			int r=0;
			int g=0;
			int b=0;

			int w=0;

			if (m_is_color) {
				RGBQUAD c;
				int wx, wy, wz;

				for (int v=iy1; v<=iy2; v++) {
					wy = INT_PRECISION;
					if (v == iy1) {
						wy = jy1;
					} else {
						if (v == iy2) wy = jy2;
					}
					for (int u=ix1; u<=ix2; u++) {
						wx = INT_PRECISION;
						if (u == ix1) {
							wx = jx1;
						} else {
							if (u == ix2) wx = jx2;
						}
						wz = (wx * wy);
						w += wz;
						c = GetPixelRGB(u,v);
						r += c.rgbRed * wz;
						g += c.rgbGreen * wz;
						b += c.rgbBlue * wz;
					}
				}
				c.rgbRed = (r/w);
				c.rgbGreen = (g/w);
				c.rgbBlue = (b/w);
				SetPixelRGB(x, c);
			} else if (m_little_endian) {
				unsigned int c = 0;
				int wx, wy, wz;

				for (int v=iy1; v<=iy2; v++) {
					wy = INT_PRECISION;
					if (v == iy1) {
						wy = jy1;
					} else {
						if (v == iy2) wy = jy2;
					}
					for (int u=ix1; u<=ix2; u++) {
						wx = INT_PRECISION;
						if (u == ix1) {
							wx = jx1;
						} else {
							if (u == ix2) wx = jx2;
						}
						wz = (wx * wy);
						w += wz;
						c += GetPixelGreyLE(u,v) * wz;;
					}
				}
				c = c / w;
				SetPixelGrey(x, c);
			} else {
				unsigned int c = 0;
				int wx, wy, wz;

				for (int v=iy1; v<=iy2; v++) {
					wy = INT_PRECISION;
					if (v == iy1) {
						wy = jy1;
					} else {
						if (v == iy2) wy = jy2;
					}
					for (int u=ix1; u<=ix2; u++) {
						wx = INT_PRECISION;
						if (u == ix1) {
							wx = jx1;
						} else {
							if (u == ix2) wx = jx2;
						}
						wz = (wx * wy);
						w += wz;
						c += GetPixelGreyBE(u,v) * wz;;
					}
				}
				c = c / w;
				SetPixelGrey(x, c);	
			}
		}
		writer.write_scanline();
	}
	writer.close();
	if (m_tif) {
		TIFFClose(m_tif);
	}
	m_tif = 0;
	return true;
}