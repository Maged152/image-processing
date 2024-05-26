# Match Template

## Description
Template matching is a technique for finding areas of an image that match 
a given template image (a smaller image patch) using the specified method 
and stores the comparison results in result

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<ImageFormat::GRAY, float> MatchTemplate(
		const Image<frmt, T>& in,
		const Image<frmt, T>& templ,
		const TemplateMatchFlag mode,
		const Image<ImageFormat::GRAY, uint8_t>& mask);
}
```



## Parameters

| Name    | Type                | Description                                                                                        |
|---------|---------------------|---------------------------------------------------------------------------------------------------|
| `in`    | `Image`             | The input image.                                                                                   |
| `templ` | `Image`             | The template image (patch) to be matched within the input image.                      |
| `mode`  | `TemplateMatchFlag` | The method used for template matching.  |
| `mask`  | `Image`             | mask image to restrict matching to certain regions.                           |


```c++
namespace qlm
{
	enum class TemplateMatchFlag
	{
		SQDIFF,
		SQDIFF_NORMED,
		CCORR,
		CCORR_NORMED
	};
}
```

## Return Value
The function returns an Image<ImageFormat::GRAY, float>, 
where each pixel represents the degree of match (similarity)
between the template and the corresponding region in the input image.


* [Example](../../../Examples/Features%20Detection/MatchTemplate)
* You can check the implementation [here](../../../../source/MatchTemplate.cpp)
