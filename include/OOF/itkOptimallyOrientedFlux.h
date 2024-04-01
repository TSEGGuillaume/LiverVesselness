#ifndef _itkOptimallyOrientedFlux_h
#define _itkOptimallyOrientedFlux_h

#include "itkImageToImageFilter.h"
#include "itkForwardFFTImageFilter.h"
#include "itkFFTShiftImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "itkComplexToRealImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkMultiplyImageFilter.h"
#include "itkInverseFFTImageFilter.h"
#include "itkSymmetricSecondRankTensor.h"

#include "itkFFTWForwardFFTImageFilter.h"

#include "boost/math/special_functions/bessel.hpp"

#include <vector>

namespace itk{
    template <typename TInputImage, typename TOutputImage > 
    class ITK_TEMPLATE_EXPORT OptimallyOrientedFlux:public ImageToImageFilter<TInputImage,TOutputImage>
    {
        public:

        ITK_DISALLOW_COPY_AND_ASSIGN(OptimallyOrientedFlux);

        /** Standard self type alias **/
        using Self = OptimallyOrientedFlux;
        using Superclass = ImageToImageFilter<TInputImage,TOutputImage>;
        using Pointer = SmartPointer<Self>;
        using ConstPointer = SmartPointer<const Self>;

        itkNewMacro(Self);
        itkTypeMacro(OptimallyOrientedFlux,ImageToImageFilter);
//        itkSetMacro(Radii,std::vector<double>);
        itkGetMacro(Radii,std::vector<double>);
        void SetRadii(const std::vector<double> &v);
        /* image related type alias */
        using ImageType = TInputImage;
        using RegionType = typename TInputImage::RegionType;
        using SizeType = typename TInputImage::SizeType;
        using IndexType = typename TInputImage::IndexType;
        using PixelType = typename TInputImage::PixelType;

        using CoordImageType = typename itk::Image<double,3>;

        protected:
        OptimallyOrientedFlux();
        ~OptimallyOrientedFlux() override = default;

        void PrintSelf(std::ostream & os,Indent indent) const override;
        void GenerateData() override;

        private:

        // Debug function

        std::vector<double> m_Radii;
        float m_sigma;
        int m_responseType;
        int m_normalizationType;
        bool m_useAbsolute;


        CoordImageType::Pointer ifftShiftedCoordMatrixX(typename TInputImage::SizeType dimension,typename TInputImage::SpacingType spacing, typename TInputImage::PointType origin);
        CoordImageType::Pointer ifftShiftedCoordMatrixY(typename TInputImage::SizeType dimension,typename TInputImage::SpacingType spacing, typename TInputImage::PointType origin);
        CoordImageType::Pointer ifftShiftedCoordMatrixZ(typename TInputImage::SizeType dimension,typename TInputImage::SpacingType spacing, typename TInputImage::PointType origin);

        std::vector<CoordImageType::Pointer> ifftShiftedCoordMatrix(typename TInputImage::SizeType dimension,typename TInputImage::SpacingType spacing, typename TInputImage::PointType origin);
        CoordImageType::Pointer ifftshiftedcoordinate(typename TInputImage::SizeType dimension,int index,typename TInputImage::SpacingType spacing, typename TInputImage::PointType origin);
    };
}

#ifndef ITK_MANUAL_INSTANCIATION
    #include "itkOptimallyOrientedFlux.hxx"
#endif

#endif // _itkOptimallyOrientedFlux_h
