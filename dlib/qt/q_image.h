// Copyright (C) 2009  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_QIMAGE_H_
#define DLIB_QIMAGE_H_

#include <QImage>
#include "../algs.h"
#include "../pixel.h"
#include "../matrix/matrix_mat.h"
#include "../image_processing/generic_image.h"

namespace dlib
{

    template <
        typename pixel_type
        >
    class q_image
    {
    public:
        typedef pixel_type type;
        typedef default_memory_manager mem_manager_type;

        q_image (const QImage& img)
        {
           /* DLIB_CASSERT( img.format() ==  == pixel_traits<pixel_type>::num,
                         "The pixel type you gave doesn't match pixel used by the open cv Mat object."
                         << "\n\t img.depth():    " << img.depth() 
                         << "\n\t img.cv::DataType<typename pixel_traits<pixel_type>::basic_pixel_type>::depth: " 
                            << cv::DataType<typename pixel_traits<pixel_type>::basic_pixel_type>::depth 
                         << "\n\t img.channels(): " << img.channels() 
                         << "\n\t img.pixel_traits<pixel_type>::num: " << pixel_traits<pixel_type>::num 
                         );*/
            _img = img;
            init();
        }


        q_image (const QImage* img)
        {
            _img = *img;
            init();
        }

        q_image() : _data(0), _widthStep(0), _nr(0), _nc(0) {}

        unsigned long size () const { return static_cast<unsigned long>(_nr*_nc); }

        inline pixel_type* operator[](const long row ) 
        { 
            // make sure requires clause is not broken
            DLIB_ASSERT(0 <= row && row < nr(),
                "\tpixel_type* cv_image::operator[](row)"
                << "\n\t you have asked for an out of bounds row " 
                << "\n\t row:  " << row
                << "\n\t nr(): " << nr() 
                << "\n\t this:  " << this
                );

            return reinterpret_cast<pixel_type*>( _data + _widthStep*row);
        }

        inline const pixel_type* operator[](const long row ) const
        { 
            // make sure requires clause is not broken
            DLIB_ASSERT(0 <= row && row < nr(),
                "\tconst pixel_type* cv_image::operator[](row)"
                << "\n\t you have asked for an out of bounds row " 
                << "\n\t row:  " << row
                << "\n\t nr(): " << nr() 
                << "\n\t this:  " << this
                );

            return reinterpret_cast<const pixel_type*>( _data + _widthStep*row);
        }

        long nr() const { return _nr; }
        long nc() const { return _nc; }
        long width_step() const { return _widthStep; }

        q_image& operator=( const q_image& img)
        {

            _img = img._img;
            init();
            return *this;
        }

        q_image& operator=( const q_image* img)
        {
            _img = *img->_img;
            init();
            return *this;
        }


    private:

        void init ()
        {
            _data = (char*)_img.bits();
            _widthStep = _img.bytesPerLine();
            _nr = _img.height();
            _nc = _img.width();

        }

        QImage _img;
        char* _data;
        long _widthStep;
        long _nr;
        long _nc;
    };

// ----------------------------------------------------------------------------------------

    template <
        typename T
        >
    const matrix_op< op_array2d_to_mat<q_image<T> > > mat (const q_image<T>& m )
    {
        typedef op_array2d_to_mat<q_image<T> > op;
        return matrix_op<op>(op(m));
    }

// ----------------------------------------------------------------------------------------

// Define the global functions that make cv_image a proper "generic image" according to
// ../image_processing/generic_image.h
    template <typename T>
    struct image_traits<q_image<T> >
    {
        typedef T pixel_type;
    };

    template <typename T>
    inline long num_rows( const q_image<T>& img) { return img.nr(); }
    template <typename T>
    inline long num_columns( const q_image<T>& img) { return img.nc(); }

    template <typename T>
    inline void* image_data(
        q_image<T>& img
    )
    {
        if (img.size() != 0)
            return &img[0][0];
        else
            return 0;
    }

    template <typename T>
    inline const void* image_data(
        const q_image<T>& img
    )
    {
        if (img.size() != 0)
            return &img[0][0];
        else
            return 0;
    }

    template <typename T>
    inline long width_step(
        const q_image<T>& img
    ) 
    { 
        return img.width_step(); 
    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_QIMAGE_H_

