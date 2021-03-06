//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsPrerequisites.h"
#include "BsRendererMaterial.h"
#include "BsParamBlocks.h"

namespace bs { namespace ct
{
	/** @addtogroup RenderBeast
	 *  @{
	 */

	BS_PARAM_BLOCK_BEGIN(ReflectionCubeDownsampleParamDef)
		BS_PARAM_BLOCK_ENTRY(int, gCubeFace)
	BS_PARAM_BLOCK_END

	extern ReflectionCubeDownsampleParamDef gReflectionCubeDownsampleParamDef;

	/** Performs filtering on cubemap faces in order to prepare them for importance sampling. */
	class ReflectionCubeDownsampleMat : public RendererMaterial<ReflectionCubeDownsampleMat>
	{
		RMAT_DEF("ReflectionCubeDownsample.bsl")

	public:
		ReflectionCubeDownsampleMat();

		/** Downsamples the provided texture face and outputs it to the provided target. */
		void execute(const SPtr<Texture>& source, UINT32 face, const TextureSurface& surface, 
					 const SPtr<RenderTarget>& target);

	private:
		SPtr<GpuParamBlockBuffer> mParamBuffer;
		GpuParamTexture mInputTexture;
	};

	BS_PARAM_BLOCK_BEGIN(ReflectionCubeImportanceSampleParamDef)
		BS_PARAM_BLOCK_ENTRY(int, gCubeFace)
		BS_PARAM_BLOCK_ENTRY(int, gMipLevel)
		BS_PARAM_BLOCK_ENTRY(int, gNumMips)
		BS_PARAM_BLOCK_ENTRY(float, gPrecomputedMipFactor)
	BS_PARAM_BLOCK_END

	extern ReflectionCubeImportanceSampleParamDef gReflectionCubeImportanceSampleParamDef;

	/** Performs importance sampling on cubemap faces in order for make them suitable for specular evaluation. */
	class ReflectionCubeImportanceSampleMat : public RendererMaterial<ReflectionCubeImportanceSampleMat>
	{
		RMAT_DEF("ReflectionCubeImportanceSample.bsl")

	public:
		ReflectionCubeImportanceSampleMat();

		/** Importance samples the provided texture face and outputs it to the provided target. */
		void execute(const SPtr<Texture>& source, UINT32 face, UINT32 mip, const SPtr<RenderTarget>& target);

	private:
		static const UINT32 NUM_SAMPLES;

		SPtr<GpuParamBlockBuffer> mParamBuffer;
		GpuParamTexture mInputTexture;
	};


	/** Helper class that handles generation and processing of textures used for reflection probes. */
	class BS_EXPORT ReflectionProbes
	{
	public:
		/**
		 * Performs filtering on the cubemap, populating its mip-maps with filtered values that can be used for
		 * evaluating specular reflections.
		 * 
		 * @param[in, out]	cubemap		Cubemap to filter. Its mip level 0 will be read, filtered and written into
		 *								other mip levels.
		 * @param[in]		scratch		Temporary cubemap texture to use for the filtering process. Must match the size of
		 *								the source cubemap. Provide null to automatically create a scratch cubemap.
		 */
		static void filterCubemapForSpecular(const SPtr<Texture>& cubemap, const SPtr<Texture>& scratch);

		/**
		 * Scales a cubemap and outputs it in the destination texture, using hardware acceleration. If both textures are the
		 * same size, performs a copy instead.
		 *
		 * @param[in]   src				Source cubemap to scale.
		 * @param[in]   srcMip			Determines which mip level of the source texture to scale.
		 * @param[in]   dst				Desination texture to output the scaled data to. Must be usable as a render target.
		 * @param[in]   dstMip			Determines which mip level of the destination texture to scale.
		 */
		static void scaleCubemap(const SPtr<Texture>& src, UINT32 srcMip, const SPtr<Texture>& dst, UINT32 dstMip);

		static const UINT32 REFLECTION_CUBEMAP_SIZE;

	private:
		/** 
		 * Downsamples a cubemap using hardware bilinear filtering. 
		 * 
		 * @param[in]	src		Cubemap to downsample.
		 * @param[in]   srcMip	Determines which mip level of the source texture to downsample.
		 * @param[in]   dst		Desination texture to output the scaled data to. Must be usable as a render target.
		 * @param[in]   dstMip			Determines which mip level of the destination texture to scale.
		 */
		static void downsampleCubemap(const SPtr<Texture>& src, UINT32 srcMip, const SPtr<Texture>& dst, UINT32 dstMip);
	};

	/** @} */
}}
