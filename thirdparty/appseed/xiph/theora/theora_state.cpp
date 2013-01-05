/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggTheora SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE Theora SOURCE CODE IS COPYRIGHT (C) 2002-2009                *
 * by the Xiph.Org Foundation and contributors http://www.xiph.org/ *
 *                                                                  *
 ********************************************************************

  function:
    last mod: $Id: state.c 16503 2009-08-22 18:14:02Z giles $

 ********************************************************************/
#include "framework.h"


/*Returns the fragment index of the top-left block in a macro block.
  This can be used to test whether or not the whole macro block is valid.
  _sb_map: The super block map.
  _quadi:  The quadrant number.
  Return: The index of the fragment of the upper left block in the macro
   block, or -1 if the block lies outside the coded frame.*/
static ptrdiff_t oc_sb_quad_top_left_frag(oc_sb_map_quad _sb_map[4],int32_t _quadi){
  /*It so happens that under the Hilbert curve ordering described below, the
     upper-left block in each macro block is at index 0, except in macro block
     3, where it is at index 2.*/
  return _sb_map[_quadi][_quadi&_quadi<<1];
}

/*Fills in the mapping from block positions to fragment numbers for a single
   color plane.
  This function also fills in the "valid" flag of each quadrant in the super
   block flags.
  _sb_maps:  The array of super block maps for the color plane.
  _sb_flags: The array of super block flags for the color plane.
  _frag0:    The index of the first fragment in the plane.
  _hfrags:   The number of horizontal fragments in a coded frame.
  _vfrags:   The number of vertical fragments in a coded frame.*/
static void oc_sb_create_plane_mapping(oc_sb_map _sb_maps[],
 oc_sb_flags _sb_flags[],ptrdiff_t _frag0,int32_t _hfrags,int32_t _vfrags){
  /*Contains the (macro_block,block) indices for a 4x4 grid of
     fragments.
    The pattern is a 4x4 Hilbert space-filling curve.
    A Hilbert curve has the nice property that as the curve grows larger, its
     fractal dimension approaches 2.
    The intuition is that nearby blocks in the curve are also close spatially,
     with the previous element always an immediate neighbor, so that runs of
     blocks should be well correlated.*/
  static const int32_t SB_MAP[4][4][2]={
    {{0,0},{0,1},{3,2},{3,3}},
    {{0,3},{0,2},{3,1},{3,0}},
    {{1,0},{1,3},{2,0},{2,3}},
    {{1,1},{1,2},{2,1},{2,2}}
  };
  ptrdiff_t  yfrag;
  uint32_t   sbi;
  int32_t        y;
  sbi=0;
  yfrag=_frag0;
  for(y=0;;y+=4){
    int32_t imax;
    int32_t x;
    /*Figure out how many columns of blocks in this super block lie within the
       image.*/
    imax=_vfrags-y;
    if(imax>4)imax=4;
    else if(imax<=0)break;
    for(x=0;;x+=4,sbi++){
      ptrdiff_t xfrag;
      int32_t       jmax;
      int32_t       quadi;
      int32_t       i;
      /*Figure out how many rows of blocks in this super block lie within the
         image.*/
      jmax=_hfrags-x;
      if(jmax>4)jmax=4;
      else if(jmax<=0)break;
      /*By default, set all fragment indices to -1.*/
      memset(_sb_maps[sbi][0],0xFF,sizeof(_sb_maps[sbi]));
      /*Fill in the fragment map for this super block.*/
      xfrag=yfrag+x;
      for(i=0;i<imax;i++){
        int32_t j;
        for(j=0;j<jmax;j++){
          _sb_maps[sbi][SB_MAP[i][j][0]][SB_MAP[i][j][1]]=xfrag+j;
        }
        xfrag+=_hfrags;
      }
      /*Mark which quadrants of this super block lie within the image.*/
      for(quadi=0;quadi<4;quadi++){
        _sb_flags[sbi].quad_valid|=
         (oc_sb_quad_top_left_frag(_sb_maps[sbi],quadi)>=0)<<quadi;
      }
    }
    yfrag+=_hfrags<<2;
  }
}

/*Fills in the Y plane fragment map for a macro block given the fragment
   coordinates of its upper-left hand corner.
  _mb_map:    The macro block map to fill.
  _fplane: The description of the Y plane.
  _xfrag0: The X location of the upper-left hand fragment in the luma plane.
  _yfrag0: The Y location of the upper-left hand fragment in the luma plane.*/
static void oc_mb_fill_ymapping(oc_mb_map_plane _mb_map[3],
 const oc_fragment_plane *_fplane,int32_t _xfrag0,int32_t _yfrag0){
  int32_t i;
  int32_t j;
  for(i=0;i<2;i++)for(j=0;j<2;j++){
    _mb_map[0][i<<1|j]=(_yfrag0+i)*(ptrdiff_t)_fplane->nhfrags+_xfrag0+j;
  }
}

/*Fills in the chroma plane fragment maps for a macro block.
  This version is for use with chroma decimated in the X and Y directions
   (4:2:0).
  _mb_map:  The macro block map to fill.
  _fplanes: The descriptions of the fragment planes.
  _xfrag0:  The X location of the upper-left hand fragment in the luma plane.
  _yfrag0:  The Y location of the upper-left hand fragment in the luma plane.*/
static void oc_mb_fill_cmapping00(oc_mb_map_plane _mb_map[3],
 const oc_fragment_plane _fplanes[3],int32_t _xfrag0,int32_t _yfrag0){
  ptrdiff_t fragi;
  _xfrag0>>=1;
  _yfrag0>>=1;
  fragi=_yfrag0*(ptrdiff_t)_fplanes[1].nhfrags+_xfrag0;
  _mb_map[1][0]=fragi+_fplanes[1].froffset;
  _mb_map[2][0]=fragi+_fplanes[2].froffset;
}

/*Fills in the chroma plane fragment maps for a macro block.
  This version is for use with chroma decimated in the Y direction.
  _mb_map:  The macro block map to fill.
  _fplanes: The descriptions of the fragment planes.
  _xfrag0:  The X location of the upper-left hand fragment in the luma plane.
  _yfrag0:  The Y location of the upper-left hand fragment in the luma plane.*/
static void oc_mb_fill_cmapping01(oc_mb_map_plane _mb_map[3],
 const oc_fragment_plane _fplanes[3],int32_t _xfrag0,int32_t _yfrag0){
  ptrdiff_t fragi;
  int32_t       j;
  _yfrag0>>=1;
  fragi=_yfrag0*(ptrdiff_t)_fplanes[1].nhfrags+_xfrag0;
  for(j=0;j<2;j++){
    _mb_map[1][j]=fragi+_fplanes[1].froffset;
    _mb_map[2][j]=fragi+_fplanes[2].froffset;
    fragi++;
  }
}

/*Fills in the chroma plane fragment maps for a macro block.
  This version is for use with chroma decimated in the X direction (4:2:2).
  _mb_map:  The macro block map to fill.
  _fplanes: The descriptions of the fragment planes.
  _xfrag0:  The X location of the upper-left hand fragment in the luma plane.
  _yfrag0:  The Y location of the upper-left hand fragment in the luma plane.*/
static void oc_mb_fill_cmapping10(oc_mb_map_plane _mb_map[3],
 const oc_fragment_plane _fplanes[3],int32_t _xfrag0,int32_t _yfrag0){
  ptrdiff_t fragi;
  int32_t       i;
  _xfrag0>>=1;
  fragi=_yfrag0*(ptrdiff_t)_fplanes[1].nhfrags+_xfrag0;
  for(i=0;i<2;i++){
    _mb_map[1][i<<1]=fragi+_fplanes[1].froffset;
    _mb_map[2][i<<1]=fragi+_fplanes[2].froffset;
    fragi+=_fplanes[1].nhfrags;
  }
}

/*Fills in the chroma plane fragment maps for a macro block.
  This version is for use with no chroma decimation (4:4:4).
  This uses the already filled-in luma plane values.
  _mb_map:  The macro block map to fill.
  _fplanes: The descriptions of the fragment planes.*/
static void oc_mb_fill_cmapping11(oc_mb_map_plane _mb_map[3],
 const oc_fragment_plane _fplanes[3]){
  int32_t k;
  for(k=0;k<4;k++){
    _mb_map[1][k]=_mb_map[0][k]+_fplanes[1].froffset;
    _mb_map[2][k]=_mb_map[0][k]+_fplanes[2].froffset;
  }
}

/*The function type used to fill in the chroma plane fragment maps for a
   macro block.
  _mb_map:  The macro block map to fill.
  _fplanes: The descriptions of the fragment planes.
  _xfrag0:  The X location of the upper-left hand fragment in the luma plane.
  _yfrag0:  The Y location of the upper-left hand fragment in the luma plane.*/
typedef void (*oc_mb_fill_cmapping_func)(oc_mb_map_plane _mb_map[3],
 const oc_fragment_plane _fplanes[3],int32_t _xfrag0,int32_t _yfrag0);

/*A table of functions used to fill in the chroma plane fragment maps for a
   macro block for each type of chrominance decimation.*/
static const oc_mb_fill_cmapping_func OC_MB_FILL_CMAPPING_TABLE[4]={
  oc_mb_fill_cmapping00,
  oc_mb_fill_cmapping01,
  oc_mb_fill_cmapping10,
  (oc_mb_fill_cmapping_func)oc_mb_fill_cmapping11
};

/*Fills in the mapping from macro blocks to their corresponding fragment
   numbers in each plane.
  _mb_maps:   The list of macro block maps.
  _mb_modes:  The list of macro block modes; macro blocks completely outside
               the coded region are marked invalid.
  _fplanes:   The descriptions of the fragment planes.
  _pixel_fmt: The chroma decimation type.*/
static void oc_mb_create_mapping(oc_mb_map _mb_maps[],
 signed char _mb_modes[],const oc_fragment_plane _fplanes[3],int32_t _pixel_fmt){
  oc_mb_fill_cmapping_func  mb_fill_cmapping;
  uint32_t                  sbi;
  int32_t                       y;
  mb_fill_cmapping=OC_MB_FILL_CMAPPING_TABLE[_pixel_fmt];
  /*Loop through the luma plane super blocks.*/
  for(sbi=y=0;y<_fplanes[0].nvfrags;y+=4){
    int32_t x;
    for(x=0;x<_fplanes[0].nhfrags;x+=4,sbi++){
      int32_t ymb;
      /*Loop through the macro blocks in each super block in display order.*/
      for(ymb=0;ymb<2;ymb++){
        int32_t xmb;
        for(xmb=0;xmb<2;xmb++){
          uint32_t mbi;
          int32_t      mbx;
          int32_t      mby;
          mbi=sbi<<2|OC_MB_MAP[ymb][xmb];
          mbx=x|xmb<<1;
          mby=y|ymb<<1;
          /*Initialize fragment indices to -1.*/
          memset(_mb_maps[mbi],0xFF,sizeof(_mb_maps[mbi]));
          /*Make sure this macro block is within the encoded region.*/
          if(mbx>=_fplanes[0].nhfrags||mby>=_fplanes[0].nvfrags){
            _mb_modes[mbi]=OC_MODE_INVALID;
            continue;
          }
          /*Fill in the fragment indices for the luma plane.*/
          oc_mb_fill_ymapping(_mb_maps[mbi],_fplanes,mbx,mby);
          /*Fill in the fragment indices for the chroma planes.*/
          (*mb_fill_cmapping)(_mb_maps[mbi],_fplanes,mbx,mby);
        }
      }
    }
  }
}

/*Marks the fragments which fall all or partially outside the displayable
   region of the frame.
  _state: The Theora state containing the fragments to be marked.*/
static void oc_state_border_init(oc_theora_state *_state){
  oc_fragment       *frag;
  oc_fragment       *yfrag_end;
  oc_fragment       *xfrag_end;
  oc_fragment_plane *fplane;
  int32_t                crop_x0;
  int32_t                crop_y0;
  int32_t                crop_xf;
  int32_t                crop_yf;
  int32_t                pli;
  int32_t                y;
  int32_t                x;
  /*The method we use here is slow, but the code is dead simple and handles
     all the special cases easily.
    We only ever need to do it once.*/
  /*Loop through the fragments, marking those completely outside the
     displayable region and constructing a border mask for those that straddle
     the border.*/
  _state->nborders=0;
  yfrag_end=frag=_state->frags;
  for(pli=0;pli<3;pli++){
    fplane=_state->fplanes+pli;
    /*Set up the cropping rectangle for this plane.*/
    crop_x0=_state->info.pic_x;
    crop_xf=_state->info.pic_x+_state->info.pic_width;
    crop_y0=_state->info.pic_y;
    crop_yf=_state->info.pic_y+_state->info.pic_height;
    if(pli>0){
      if(!(_state->info.pixel_fmt&1)){
        crop_x0=crop_x0>>1;
        crop_xf=crop_xf+1>>1;
      }
      if(!(_state->info.pixel_fmt&2)){
        crop_y0=crop_y0>>1;
        crop_yf=crop_yf+1>>1;
      }
    }
    y=0;
    for(yfrag_end+=fplane->nfrags;frag<yfrag_end;y+=8){
      x=0;
      for(xfrag_end=frag+fplane->nhfrags;frag<xfrag_end;frag++,x+=8){
        /*First check to see if this fragment is completely outside the
           displayable region.*/
        /*Note the special checks for an empty cropping rectangle.
          This guarantees that if we count a fragment as straddling the
           border below, at least one pixel in the fragment will be inside
           the displayable region.*/
        if(x+8<=crop_x0||crop_xf<=x||y+8<=crop_y0||crop_yf<=y||
         crop_x0>=crop_xf||crop_y0>=crop_yf){
          frag->invalid=1;
        }
        /*Otherwise, check to see if it straddles the border.*/
        else if(x<crop_x0&&crop_x0<x+8||x<crop_xf&&crop_xf<x+8||
         y<crop_y0&&crop_y0<y+8||y<crop_yf&&crop_yf<y+8){
          ogg_int64_t mask;
          int32_t         npixels;
          int32_t         i;
          mask=npixels=0;
          for(i=0;i<8;i++){
            int32_t j;
            for(j=0;j<8;j++){
              if(x+j>=crop_x0&&x+j<crop_xf&&y+i>=crop_y0&&y+i<crop_yf){
                mask|=(ogg_int64_t)1<<(i<<3|j);
                npixels++;
              }
            }
          }
          /*Search the fragment array for border info with the same pattern.
            In general, there will be at most 8 different patterns (per
             plane).*/
          for(i=0;;i++){
            if(i>=_state->nborders){
              _state->nborders++;
              _state->borders[i].mask=mask;
              _state->borders[i].npixels=npixels;
            }
            else if(_state->borders[i].mask!=mask)continue;
            frag->borderi=i;
            break;
          }
        }
        else frag->borderi=-1;
      }
    }
  }
}

static int32_t oc_state_frarray_init(oc_theora_state *_state){
  int32_t       yhfrags;
  int32_t       yvfrags;
  int32_t       chfrags;
  int32_t       cvfrags;
  ptrdiff_t yfrags;
  ptrdiff_t cfrags;
  ptrdiff_t nfrags;
  uint32_t  yhsbs;
  uint32_t  yvsbs;
  uint32_t  chsbs;
  uint32_t  cvsbs;
  uint32_t  ysbs;
  uint32_t  csbs;
  uint32_t  nsbs;
  size_t    nmbs;
  int32_t       hdec;
  int32_t       vdec;
  int32_t       pli;
  /*Figure out the number of fragments in each plane.*/
  /*These parameters have already been validated to be multiples of 16.*/
  yhfrags=_state->info.frame_width>>3;
  yvfrags=_state->info.frame_height>>3;
  hdec=!(_state->info.pixel_fmt&1);
  vdec=!(_state->info.pixel_fmt&2);
  chfrags=yhfrags+hdec>>hdec;
  cvfrags=yvfrags+vdec>>vdec;
  yfrags=yhfrags*(ptrdiff_t)yvfrags;
  cfrags=chfrags*(ptrdiff_t)cvfrags;
  nfrags=yfrags+2*cfrags;
  /*Figure out the number of super blocks in each plane.*/
  yhsbs=yhfrags+3>>2;
  yvsbs=yvfrags+3>>2;
  chsbs=chfrags+3>>2;
  cvsbs=cvfrags+3>>2;
  ysbs=yhsbs*yvsbs;
  csbs=chsbs*cvsbs;
  nsbs=ysbs+2*csbs;
  nmbs=(size_t)ysbs<<2;
  /*Check for overflow.
    We support the ridiculous upper limits of the specification (1048560 by
     1048560, or 3 TB frames) if the target architecture has 64-bit pointers,
     but for those with 32-bit pointers (or smaller!) we have to check.
    If the caller wants to prevent denial-of-service by imposing a more
     reasonable upper limit on the size of attempted allocations, they must do
     so themselves; we have no platform independent way to determine how much
     system memory there is nor an application-independent way to decide what a
     "reasonable" allocation is.*/
  if(yfrags/yhfrags!=yvfrags||2*cfrags<cfrags||nfrags<yfrags||
   ysbs/yhsbs!=yvsbs||2*csbs<csbs||nsbs<ysbs||nmbs>>2!=ysbs){
    return TH_EIMPL;
  }
  /*Initialize the fragment array.*/
  _state->fplanes[0].nhfrags=yhfrags;
  _state->fplanes[0].nvfrags=yvfrags;
  _state->fplanes[0].froffset=0;
  _state->fplanes[0].nfrags=yfrags;
  _state->fplanes[0].nhsbs=yhsbs;
  _state->fplanes[0].nvsbs=yvsbs;
  _state->fplanes[0].sboffset=0;
  _state->fplanes[0].nsbs=ysbs;
  _state->fplanes[1].nhfrags=_state->fplanes[2].nhfrags=chfrags;
  _state->fplanes[1].nvfrags=_state->fplanes[2].nvfrags=cvfrags;
  _state->fplanes[1].froffset=yfrags;
  _state->fplanes[2].froffset=yfrags+cfrags;
  _state->fplanes[1].nfrags=_state->fplanes[2].nfrags=cfrags;
  _state->fplanes[1].nhsbs=_state->fplanes[2].nhsbs=chsbs;
  _state->fplanes[1].nvsbs=_state->fplanes[2].nvsbs=cvsbs;
  _state->fplanes[1].sboffset=ysbs;
  _state->fplanes[2].sboffset=ysbs+csbs;
  _state->fplanes[1].nsbs=_state->fplanes[2].nsbs=csbs;
  _state->nfrags=nfrags;
  _state->frags=(oc_fragment *) _ogg_calloc(nfrags,sizeof(*_state->frags));
  _state->frag_mvs=(oc_mv *) _ogg_malloc(nfrags*sizeof(*_state->frag_mvs));
  _state->nsbs=nsbs;
  _state->sb_maps=(oc_sb_map *) _ogg_malloc(nsbs*sizeof(*_state->sb_maps));
  _state->sb_flags= (oc_sb_flags *) _ogg_calloc(nsbs,sizeof(*_state->sb_flags));
  _state->nhmbs=yhsbs<<1;
  _state->nvmbs=yvsbs<<1;
  _state->nmbs=nmbs;
  _state->mb_maps= (oc_mb_map *) _ogg_calloc(nmbs,sizeof(*_state->mb_maps));
  _state->mb_modes=(signed char *) _ogg_calloc(nmbs,sizeof(*_state->mb_modes));
  _state->coded_fragis=(ptrdiff_t *) _ogg_malloc(nfrags*sizeof(*_state->coded_fragis));
  if(_state->frags==NULL||_state->frag_mvs==NULL||_state->sb_maps==NULL||
   _state->sb_flags==NULL||_state->mb_maps==NULL||_state->mb_modes==NULL||
   _state->coded_fragis==NULL){
    return TH_EFAULT;
  }
  /*Create the mapping from super blocks to fragments.*/
  for(pli=0;pli<3;pli++){
    oc_fragment_plane *fplane;
    fplane=_state->fplanes+pli;
    oc_sb_create_plane_mapping(_state->sb_maps+fplane->sboffset,
     _state->sb_flags+fplane->sboffset,fplane->froffset,
     fplane->nhfrags,fplane->nvfrags);
  }
  /*Create the mapping from macro blocks to fragments.*/
  oc_mb_create_mapping(_state->mb_maps,_state->mb_modes,
   _state->fplanes,_state->info.pixel_fmt);
  /*Initialize the invalid and borderi fields of each fragment.*/
  oc_state_border_init(_state);
  return 0;
}

static void oc_state_frarray_clear(oc_theora_state *_state){
  _ogg_free(_state->coded_fragis);
  _ogg_free(_state->mb_modes);
  _ogg_free(_state->mb_maps);
  _ogg_free(_state->sb_flags);
  _ogg_free(_state->sb_maps);
  _ogg_free(_state->frag_mvs);
  _ogg_free(_state->frags);
}


/*Initializes the buffers used for reconstructed frames.
  These buffers are padded with 16 extra pixels on each side, to allow
   unrestricted motion vectors without special casing the boundary.
  If chroma is decimated in either direction, the padding is reduced by a
   factor of 2 on the appropriate sides.
  _nrefs: The number of reference buffers to init; must be 3 or 4.*/
static int32_t oc_state_ref_bufs_init(oc_theora_state *_state,int32_t _nrefs){
  th_info       *info;
  uchar *ref_frame_data;
  size_t         ref_frame_data_sz;
  size_t         ref_frame_sz;
  size_t         yplane_sz;
  size_t         cplane_sz;
  int32_t            yhstride;
  int32_t            yheight;
  int32_t            chstride;
  int32_t            cheight;
  ptrdiff_t      yoffset;
  ptrdiff_t      coffset;
  ptrdiff_t     *frag_buf_offs;
  ptrdiff_t      fragi;
  int32_t            hdec;
  int32_t            vdec;
  int32_t            rfi;
  int32_t            pli;
  if(_nrefs<3||_nrefs>4)return TH_EINVAL;
  info=&_state->info;
  /*Compute the image buffer parameters for each plane.*/
  hdec=!(info->pixel_fmt&1);
  vdec=!(info->pixel_fmt&2);
  yhstride=info->frame_width+2*OC_UMV_PADDING;
  yheight=info->frame_height+2*OC_UMV_PADDING;
  chstride=yhstride>>hdec;
  cheight=yheight>>vdec;
  yplane_sz=yhstride*(size_t)yheight;
  cplane_sz=chstride*(size_t)cheight;
  yoffset=OC_UMV_PADDING+OC_UMV_PADDING*(ptrdiff_t)yhstride;
  coffset=(OC_UMV_PADDING>>hdec)+(OC_UMV_PADDING>>vdec)*(ptrdiff_t)chstride;
  ref_frame_sz=yplane_sz+2*cplane_sz;
  ref_frame_data_sz=_nrefs*ref_frame_sz;
  /*Check for overflow.
    The same caveats apply as for oc_state_frarray_init().*/
  if(yplane_sz/yhstride!=yheight||2*cplane_sz<cplane_sz||
   ref_frame_sz<yplane_sz||ref_frame_data_sz/_nrefs!=ref_frame_sz){
    return TH_EIMPL;
  }
  ref_frame_data=(uchar *) _ogg_malloc(ref_frame_data_sz);
  frag_buf_offs=_state->frag_buf_offs= (ptrdiff_t *) 
   _ogg_malloc(_state->nfrags*sizeof(*frag_buf_offs));
  if(ref_frame_data==NULL||frag_buf_offs==NULL){
    _ogg_free(frag_buf_offs);
    _ogg_free(ref_frame_data);
    return TH_EFAULT;
  }
  /*Set up the width, height and stride for the image buffers.*/
  _state->ref_frame_bufs[0][0].width=info->frame_width;
  _state->ref_frame_bufs[0][0].height=info->frame_height;
  _state->ref_frame_bufs[0][0].stride=yhstride;
  _state->ref_frame_bufs[0][1].width=_state->ref_frame_bufs[0][2].width=
   info->frame_width>>hdec;
  _state->ref_frame_bufs[0][1].height=_state->ref_frame_bufs[0][2].height=
   info->frame_height>>vdec;
  _state->ref_frame_bufs[0][1].stride=_state->ref_frame_bufs[0][2].stride=
   chstride;
  for(rfi=1;rfi<_nrefs;rfi++){
    memcpy(_state->ref_frame_bufs[rfi],_state->ref_frame_bufs[0],
     sizeof(_state->ref_frame_bufs[0]));
  }
  /*Set up the data pointers for the image buffers.*/
  for(rfi=0;rfi<_nrefs;rfi++){
    _state->ref_frame_data[rfi]=ref_frame_data;
    _state->ref_frame_bufs[rfi][0].data=ref_frame_data+yoffset;
    ref_frame_data+=yplane_sz;
    _state->ref_frame_bufs[rfi][1].data=ref_frame_data+coffset;
    ref_frame_data+=cplane_sz;
    _state->ref_frame_bufs[rfi][2].data=ref_frame_data+coffset;
    ref_frame_data+=cplane_sz;
    /*Flip the buffer upside down.
      This allows us to decode Theora's bottom-up frames in their natural
       order, yet return a top-down buffer with a positive stride to the user.*/
    oc_ycbcr_buffer_flip(_state->ref_frame_bufs[rfi],
     _state->ref_frame_bufs[rfi]);
  }
  _state->ref_ystride[0]=-yhstride;
  _state->ref_ystride[1]=_state->ref_ystride[2]=-chstride;
  /*Initialize the fragment buffer offsets.*/
  ref_frame_data=_state->ref_frame_data[0];
  fragi=0;
  for(pli=0;pli<3;pli++){
    th_img_plane      *iplane;
    oc_fragment_plane *fplane;
    uchar     *vpix;
    ptrdiff_t          stride;
    ptrdiff_t          vfragi_end;
    int32_t                nhfrags;
    iplane=_state->ref_frame_bufs[0]+pli;
    fplane=_state->fplanes+pli;
    vpix=iplane->data;
    vfragi_end=fplane->froffset+fplane->nfrags;
    nhfrags=fplane->nhfrags;
    stride=iplane->stride;
    while(fragi<vfragi_end){
      ptrdiff_t      hfragi_end;
      uchar *hpix;
      hpix=vpix;
      for(hfragi_end=fragi+nhfrags;fragi<hfragi_end;fragi++){
        frag_buf_offs[fragi]=hpix-ref_frame_data;
        hpix+=8;
      }
      vpix+=stride<<3;
    }
  }
  /*Initialize the reference frame indices.*/
  _state->ref_frame_idx[OC_FRAME_GOLD]=
   _state->ref_frame_idx[OC_FRAME_PREV]=
   _state->ref_frame_idx[OC_FRAME_SELF]=-1;
  _state->ref_frame_idx[OC_FRAME_IO]=_nrefs>3?3:-1;
  return 0;
}

static void oc_state_ref_bufs_clear(oc_theora_state *_state){
  _ogg_free(_state->frag_buf_offs);
  _ogg_free(_state->ref_frame_data[0]);
}


void oc_state_vtable_init_c(oc_theora_state *_state){
  _state->opt_vtable.frag_copy=oc_frag_copy_c;
  _state->opt_vtable.frag_recon_intra=oc_frag_recon_intra_c;
  _state->opt_vtable.frag_recon_inter=oc_frag_recon_inter_c;
  _state->opt_vtable.frag_recon_inter2=oc_frag_recon_inter2_c;
  _state->opt_vtable.idct8x8=oc_idct8x8_c;
  _state->opt_vtable.state_frag_recon=oc_state_frag_recon_c;
  _state->opt_vtable.state_frag_copy_list=oc_state_frag_copy_list_c;
  _state->opt_vtable.state_loop_filter_frag_rows=
   oc_state_loop_filter_frag_rows_c;
  _state->opt_vtable.restore_fpu=oc_restore_fpu_c;
  _state->opt_data.dct_fzig_zag=OC_FZIG_ZAG;
}

/*Initialize the accelerated function pointers.*/
void oc_state_vtable_init(oc_theora_state *_state){
#if defined(OC_X86_ASM)
  oc_state_vtable_init_x86(_state);
#else
  oc_state_vtable_init_c(_state);
#endif
}


int32_t oc_state_init(oc_theora_state *_state,const th_info *_info,int32_t _nrefs){
  int32_t ret;
  /*First validate the parameters.*/
  if(_info==NULL)return TH_EFAULT;
  /*The width and height of the encoded frame must be multiples of 16.
    They must also, when divided by 16, fit into a 16-bit uint32_t integer.
    The displayable frame offset coordinates must fit into an 8-bit uint32_t
     integer.
    Note that the offset Y in the API is specified on the opposite side from
     how it is specified in the bitstream, because the Y axis is flipped in
     the bitstream.
    The displayable frame must fit inside the encoded frame.
    The color space must be one known by the encoder.*/
  if((_info->frame_width&0xF)||(_info->frame_height&0xF)||
   _info->frame_width<=0||_info->frame_width>=0x100000||
   _info->frame_height<=0||_info->frame_height>=0x100000||
   _info->pic_x+_info->pic_width>_info->frame_width||
   _info->pic_y+_info->pic_height>_info->frame_height||
   _info->pic_x>255||_info->frame_height-_info->pic_height-_info->pic_y>255||
   /*Note: the following <0 comparisons may generate spurious warnings on
      platforms where enums are uint32_t.
     We could cast them to uint32_t and just use the following >= comparison,
      but there are a number of compilers which will mis-optimize this.
     It's better to live with the spurious warnings.*/
   _info->colorspace<0||_info->colorspace>=TH_CS_NSPACES||
   _info->pixel_fmt<0||_info->pixel_fmt>=TH_PF_NFORMATS){
    return TH_EINVAL;
  }
  memset(_state,0,sizeof(*_state));
  memcpy(&_state->info,_info,sizeof(*_info));
  /*Invert the sense of pic_y to match Theora's right-handed coordinate
     system.*/
  _state->info.pic_y=_info->frame_height-_info->pic_height-_info->pic_y;
  _state->frame_type=OC_UNKWN_FRAME;
  oc_state_vtable_init(_state);
  ret=oc_state_frarray_init(_state);
  if(ret>=0)ret=oc_state_ref_bufs_init(_state,_nrefs);
  if(ret<0){
    oc_state_frarray_clear(_state);
    return ret;
  }
  /*If the keyframe_granule_shift is out of range, use the maximum allowable
     value.*/
  if(_info->keyframe_granule_shift<0||_info->keyframe_granule_shift>31){
    _state->info.keyframe_granule_shift=31;
  }
  _state->keyframe_num=0;
  _state->curframe_num=-1;
  /*3.2.0 streams mark the frame index instead of the frame count.
    This was changed with stream version 3.2.1 to conform to other Ogg
     codecs.
    We add an extra bias when computing granule positions for new streams.*/
  _state->granpos_bias=TH_VERSION_CHECK(_info,3,2,1);
  return 0;
}

void oc_state_clear(oc_theora_state *_state){
  oc_state_ref_bufs_clear(_state);
  oc_state_frarray_clear(_state);
}


/*Duplicates the pixels on the border of the image plane out into the
   surrounding padding for use by unrestricted motion vectors.
  This function only adds the left and right borders, and only for the fragment
   rows specified.
  _refi: The index of the reference buffer to pad.
  _pli:  The color plane.
  _y0:   The Y coordinate of the first row to pad.
  _yend: The Y coordinate of the row to stop padding at.*/
void oc_state_borders_fill_rows(oc_theora_state *_state,int32_t _refi,int32_t _pli,
 int32_t _y0,int32_t _yend){
  th_img_plane  *iplane;
  uchar *apix;
  uchar *bpix;
  uchar *epix;
  int32_t            stride;
  int32_t            hpadding;
  hpadding=OC_UMV_PADDING>>safe_int_bool(_pli!=0&&!(_state->info.pixel_fmt&1));
  iplane=_state->ref_frame_bufs[_refi]+_pli;
  stride=iplane->stride;
  apix=iplane->data+_y0*(ptrdiff_t)stride;
  bpix=apix+iplane->width-1;
  epix=iplane->data+_yend*(ptrdiff_t)stride;
  /*Note the use of != instead of <, which allows the stride to be negative.*/
  while(apix!=epix){
    memset(apix-hpadding,apix[0],hpadding);
    memset(bpix+1,bpix[0],hpadding);
    apix+=stride;
    bpix+=stride;
  }
}

/*Duplicates the pixels on the border of the image plane out into the
   surrounding padding for use by unrestricted motion vectors.
  This function only adds the top and bottom borders, and must be called after
   the left and right borders are added.
  _refi:      The index of the reference buffer to pad.
  _pli:       The color plane.*/
void oc_state_borders_fill_caps(oc_theora_state *_state,int32_t _refi,int32_t _pli){
  th_img_plane  *iplane;
  uchar *apix;
  uchar *bpix;
  uchar *epix;
  int32_t            stride;
  int32_t            hpadding;
  int32_t            vpadding;
  int32_t            fullw;
  hpadding = OC_UMV_PADDING >> safe_int_bool(_pli != 0 && !(_state->info.pixel_fmt & 1));
  vpadding = OC_UMV_PADDING >> safe_int_bool(_pli != 0 && !(_state->info.pixel_fmt & 2));
  iplane=_state->ref_frame_bufs[_refi]+_pli;
  stride=iplane->stride;
  fullw=iplane->width+(hpadding<<1);
  apix=iplane->data-hpadding;
  bpix=iplane->data+(iplane->height-1)*(ptrdiff_t)stride-hpadding;
  epix=apix-stride*(ptrdiff_t)vpadding;
  while(apix!=epix){
    memcpy(apix-stride,apix,fullw);
    memcpy(bpix+stride,bpix,fullw);
    apix-=stride;
    bpix+=stride;
  }
}

/*Duplicates the pixels on the border of the given reference image out into
   the surrounding padding for use by unrestricted motion vectors.
  _state: The context containing the reference buffers.
  _refi:  The index of the reference buffer to pad.*/
void oc_state_borders_fill(oc_theora_state *_state,int32_t _refi){
  int32_t pli;
  for(pli=0;pli<3;pli++){
    oc_state_borders_fill_rows(_state,_refi,pli,0,
     _state->ref_frame_bufs[_refi][pli].height);
    oc_state_borders_fill_caps(_state,_refi,pli);
  }
}

/*Determines the offsets in an image buffer to use for motion compensation.
  _state:   The Theora state the offsets are to be computed with.
  _offsets: Returns the offset for the buffer(s).
            _offsets[0] is always set.
            _offsets[1] is set if the motion vector has non-zero fractional
             components.
  _pli:     The color plane index.
  _dx:      The X component of the motion vector.
  _dy:      The Y component of the motion vector.
  Return: The number of offsets returned: 1 or 2.*/
int32_t oc_state_get_mv_offsets(const oc_theora_state *_state,int32_t _offsets[2],
 int32_t _pli,int32_t _dx,int32_t _dy){
  /*Here is a brief description of how Theora handles motion vectors:
    Motion vector components are specified to half-pixel accuracy in
     undecimated directions of each plane, and quarter-pixel accuracy in
     decimated directions.
    Integer parts are extracted by dividing (not shifting) by the
     appropriate amount, with truncation towards zero.
    These integer values are used to calculate the first offset.

    If either of the fractional parts are non-zero, then a second offset is
     computed.
    No third or fourth offsets are computed, even if both components have
     non-zero fractional parts.
    The second offset is computed by dividing (not shifting) by the
     appropriate amount, always truncating _away_ from zero.*/
#if 0
  /*This version of the code doesn't use any tables, but is slower.*/
  int32_t ystride;
  int32_t xprec;
  int32_t yprec;
  int32_t xfrac;
  int32_t yfrac;
  int32_t offs;
  ystride=_state->ref_ystride[_pli];
  /*These two variables decide whether we are in half- or quarter-pixel
     precision in each component.*/
  xprec=1+(_pli!=0&&!(_state->info.pixel_fmt&1));
  yprec=1+(_pli!=0&&!(_state->info.pixel_fmt&2));
  /*These two variables are either 0 if all the fractional bits are zero or -1
     if any of them are non-zero.*/
  xfrac=OC_SIGNMASK(-(_dx&(xprec|1)));
  yfrac=OC_SIGNMASK(-(_dy&(yprec|1)));
  offs=(_dx>>xprec)+(_dy>>yprec)*ystride;
  if(xfrac||yfrac){
    int32_t xmask;
    int32_t ymask;
    xmask=OC_SIGNMASK(_dx);
    ymask=OC_SIGNMASK(_dy);
    yfrac&=ystride;
    _offsets[0]=offs-(xfrac&xmask)+(yfrac&ymask);
    _offsets[1]=offs-(xfrac&~xmask)+(yfrac&~ymask);
    return 2;
  }
  else{
    _offsets[0]=offs;
    return 1;
  }
#else
  /*Using tables simplifies the code, and there's enough arithmetic to hide the
     latencies of the memory references.*/
  static const signed char OC_MVMAP[2][64]={
    {
          -15,-15,-14,-14,-13,-13,-12,-12,-11,-11,-10,-10, -9, -9, -8,
       -8, -7, -7, -6, -6, -5, -5, -4, -4, -3, -3, -2, -2, -1, -1,  0,
        0,  0,  1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,
        8,  8,  9,  9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15
    },
    {
           -7, -7, -7, -7, -6, -6, -6, -6, -5, -5, -5, -5, -4, -4, -4,
       -4, -3, -3, -3, -3, -2, -2, -2, -2, -1, -1, -1, -1,  0,  0,  0,
        0,  0,  0,  0,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,
        4,  4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7
    }
  };
  static const signed char OC_MVMAP2[2][64]={
    {
        -1, 0,-1,  0,-1, 0,-1,  0,-1, 0,-1,  0,-1, 0,-1,
      0,-1, 0,-1,  0,-1, 0,-1,  0,-1, 0,-1,  0,-1, 0,-1,
      0, 1, 0, 1,  0, 1, 0, 1,  0, 1, 0, 1,  0, 1, 0, 1,
      0, 1, 0, 1,  0, 1, 0, 1,  0, 1, 0, 1,  0, 1, 0, 1
    },
    {
        -1,-1,-1,  0,-1,-1,-1,  0,-1,-1,-1,  0,-1,-1,-1,
      0,-1,-1,-1,  0,-1,-1,-1,  0,-1,-1,-1,  0,-1,-1,-1,
      0, 1, 1, 1,  0, 1, 1, 1,  0, 1, 1, 1,  0, 1, 1, 1,
      0, 1, 1, 1,  0, 1, 1, 1,  0, 1, 1, 1,  0, 1, 1, 1
    }
  };
  int32_t ystride;
  int32_t qpx;
  int32_t qpy;
  int32_t mx;
  int32_t my;
  int32_t mx2;
  int32_t my2;
  int32_t offs;
  ystride=_state->ref_ystride[_pli];
  qpy=_pli!=0&&!(_state->info.pixel_fmt&2);
  my=OC_MVMAP[qpy][_dy+31];
  my2=OC_MVMAP2[qpy][_dy+31];
  qpx=_pli!=0&&!(_state->info.pixel_fmt&1);
  mx=OC_MVMAP[qpx][_dx+31];
  mx2=OC_MVMAP2[qpx][_dx+31];
  offs=my*ystride+mx;
  if(mx2||my2){
    _offsets[1]=offs+my2*ystride+mx2;
    _offsets[0]=offs;
    return 2;
  }
  _offsets[0]=offs;
  return 1;
#endif
}

void oc_state_frag_recon(const oc_theora_state *_state,ptrdiff_t _fragi,
 int32_t _pli,ogg_int16_t _dct_coeffs[64],int32_t _last_zzi,ogg_uint16_t _dc_quant){
  _state->opt_vtable.state_frag_recon(_state,_fragi,_pli,_dct_coeffs,
   _last_zzi,_dc_quant);
}

void oc_state_frag_recon_c(const oc_theora_state *_state,ptrdiff_t _fragi,
 int32_t _pli,ogg_int16_t _dct_coeffs[64],int32_t _last_zzi,ogg_uint16_t _dc_quant){
  uchar *dst;
  ptrdiff_t      frag_buf_off;
  int32_t            ystride;
  int32_t            mb_mode;
  /*Apply the inverse transform.*/
  /*Special case only having a DC component.*/
  if(_last_zzi<2){
    ogg_int16_t p;
    int32_t         ci;
    /*We round this dequant product (and not any of the others) because there's
       no iDCT rounding.*/
    p=(ogg_int16_t)(_dct_coeffs[0]*(ogg_int32_t)_dc_quant+15>>5);
    /*LOOP VECTORIZES.*/
    for(ci=0;ci<64;ci++)_dct_coeffs[ci]=p;
  }
  else{
    /*First, dequantize the DC coefficient.*/
    _dct_coeffs[0]=(ogg_int16_t)(_dct_coeffs[0]*(int32_t)_dc_quant);
    oc_idct8x8(_state,_dct_coeffs,_last_zzi);
  }
  /*Fill in the target buffer.*/
  frag_buf_off=_state->frag_buf_offs[_fragi];
  mb_mode=_state->frags[_fragi].mb_mode;
  ystride=_state->ref_ystride[_pli];
  dst=_state->ref_frame_data[_state->ref_frame_idx[OC_FRAME_SELF]]+frag_buf_off;
  if(mb_mode==OC_MODE_INTRA)oc_frag_recon_intra(_state,dst,ystride,_dct_coeffs);
  else{
    const uchar *ref;
    int32_t                  mvoffsets[2];
    ref=
     _state->ref_frame_data[_state->ref_frame_idx[OC_FRAME_FOR_MODE(mb_mode)]]
     +frag_buf_off;
    if(oc_state_get_mv_offsets(_state,mvoffsets,_pli,
     _state->frag_mvs[_fragi][0],_state->frag_mvs[_fragi][1])>1){
      oc_frag_recon_inter2(_state,
       dst,ref+mvoffsets[0],ref+mvoffsets[1],ystride,_dct_coeffs);
    }
    else oc_frag_recon_inter(_state,dst,ref+mvoffsets[0],ystride,_dct_coeffs);
  }
}

/*Copies the fragments specified by the lists of fragment indices from one
   frame to another.
  _fragis:    A pointer to a list of fragment indices.
  _nfragis:   The number of fragment indices to copy.
  _dst_frame: The reference frame to copy to.
  _src_frame: The reference frame to copy from.
  _pli:       The color plane the fragments lie in.*/
void oc_state_frag_copy_list(const oc_theora_state *_state,
 const ptrdiff_t *_fragis,ptrdiff_t _nfragis,
 int32_t _dst_frame,int32_t _src_frame,int32_t _pli){
  _state->opt_vtable.state_frag_copy_list(_state,_fragis,_nfragis,_dst_frame,
   _src_frame,_pli);
}

void oc_state_frag_copy_list_c(const oc_theora_state *_state,
 const ptrdiff_t *_fragis,ptrdiff_t _nfragis,
 int32_t _dst_frame,int32_t _src_frame,int32_t _pli){
  const ptrdiff_t     *frag_buf_offs;
  const uchar *src_frame_data;
  uchar       *dst_frame_data;
  ptrdiff_t            fragii;
  int32_t                  ystride;
  dst_frame_data=_state->ref_frame_data[_state->ref_frame_idx[_dst_frame]];
  src_frame_data=_state->ref_frame_data[_state->ref_frame_idx[_src_frame]];
  ystride=_state->ref_ystride[_pli];
  frag_buf_offs=_state->frag_buf_offs;
  for(fragii=0;fragii<_nfragis;fragii++){
    ptrdiff_t frag_buf_off;
    frag_buf_off=frag_buf_offs[_fragis[fragii]];
    oc_frag_copy(_state,dst_frame_data+frag_buf_off,
     src_frame_data+frag_buf_off,ystride);
  }
}

static void loop_filter_h(uchar *_pix,int32_t _ystride,int32_t *_bv){
  int32_t y;
  _pix-=2;
  for(y=0;y<8;y++){
    int32_t f;
    f=_pix[0]-_pix[3]+3*(_pix[2]-_pix[1]);
    /*The _bv array is used to compute the function
      f=OC_CLAMPI(OC_MINI(-_2flimit-f,0),f,OC_MAXI(_2flimit-f,0));
      where _2flimit=_state->loop_filter_limits[_state->qis[0]]<<1;*/
    f=*(_bv+(f+4>>3));
    _pix[1]=OC_CLAMP255(_pix[1]+f);
    _pix[2]=OC_CLAMP255(_pix[2]-f);
    _pix+=_ystride;
  }
}

static void loop_filter_v(uchar *_pix,int32_t _ystride,int32_t *_bv){
  int32_t x;
  _pix-=_ystride*2;
  for(x=0;x<8;x++){
    int32_t f;
    f=_pix[x]-_pix[_ystride*3+x]+3*(_pix[_ystride*2+x]-_pix[_ystride+x]);
    /*The _bv array is used to compute the function
      f=OC_CLAMPI(OC_MINI(-_2flimit-f,0),f,OC_MAXI(_2flimit-f,0));
      where _2flimit=_state->loop_filter_limits[_state->qis[0]]<<1;*/
    f=*(_bv+(f+4>>3));
    _pix[_ystride+x]=OC_CLAMP255(_pix[_ystride+x]+f);
    _pix[_ystride*2+x]=OC_CLAMP255(_pix[_ystride*2+x]-f);
  }
}

/*Initialize the bounding values array used by the loop filter.
  _bv: Storage for the array.
  Return: 0 on success, or a non-zero value if no filtering need be applied.*/
int32_t oc_state_loop_filter_init(oc_theora_state *_state,int32_t _bv[256]){
  int32_t flimit;
  int32_t i;
  flimit=_state->loop_filter_limits[_state->qis[0]];
  if(flimit==0)return 1;
  memset(_bv,0,sizeof(_bv[0])*256);
  for(i=0;i<flimit;i++){
    if(127-i-flimit>=0)_bv[127-i-flimit]=i-flimit;
    _bv[127-i]=-i;
    _bv[127+i]=i;
    if(127+i+flimit<256)_bv[127+i+flimit]=flimit-i;
  }
  return 0;
}

/*Apply the loop filter to a given set of fragment rows in the given plane.
  The filter may be run on the bottom edge, affecting pixels in the next row of
   fragments, so this row also needs to be available.
  _bv:        The bounding values array.
  _refi:      The index of the frame buffer to filter.
  _pli:       The color plane to filter.
  _fragy0:    The Y coordinate of the first fragment row to filter.
  _fragy_end: The Y coordinate of the fragment row to stop filtering at.*/
void oc_state_loop_filter_frag_rows(const oc_theora_state *_state,int32_t _bv[256],
 int32_t _refi,int32_t _pli,int32_t _fragy0,int32_t _fragy_end){
  _state->opt_vtable.state_loop_filter_frag_rows(_state,_bv,_refi,_pli,
   _fragy0,_fragy_end);
}

void oc_state_loop_filter_frag_rows_c(const oc_theora_state *_state,int32_t *_bv,
 int32_t _refi,int32_t _pli,int32_t _fragy0,int32_t _fragy_end){
  const oc_fragment_plane *fplane;
  const oc_fragment       *frags;
  const ptrdiff_t         *frag_buf_offs;
  uchar           *ref_frame_data;
  ptrdiff_t                fragi_top;
  ptrdiff_t                fragi_bot;
  ptrdiff_t                fragi0;
  ptrdiff_t                fragi0_end;
  int32_t                      ystride;
  int32_t                      nhfrags;
  _bv+=127;
  fplane=_state->fplanes+_pli;
  nhfrags=fplane->nhfrags;
  fragi_top=fplane->froffset;
  fragi_bot=fragi_top+fplane->nfrags;
  fragi0=fragi_top+_fragy0*(ptrdiff_t)nhfrags;
  fragi0_end=fragi0+(_fragy_end-_fragy0)*(ptrdiff_t)nhfrags;
  ystride=_state->ref_ystride[_pli];
  frags=_state->frags;
  frag_buf_offs=_state->frag_buf_offs;
  ref_frame_data=_state->ref_frame_data[_refi];
  /*The following loops are constructed somewhat non-intuitively on purpose.
    The main idea is: if a block boundary has at least one coded fragment on
     it, the filter is applied to it.
    However, the order that the filters are applied in matters, and VP3 chose
     the somewhat strange ordering used below.*/
  while(fragi0<fragi0_end){
    ptrdiff_t fragi;
    ptrdiff_t fragi_end;
    fragi=fragi0;
    fragi_end=fragi+nhfrags;
    while(fragi<fragi_end){
      if(frags[fragi].coded){
        uchar *ref;
        ref=ref_frame_data+frag_buf_offs[fragi];
        if(fragi>fragi0)loop_filter_h(ref,ystride,_bv);
        if(fragi0>fragi_top)loop_filter_v(ref,ystride,_bv);
        if(fragi+1<fragi_end&&!frags[fragi+1].coded){
          loop_filter_h(ref+8,ystride,_bv);
        }
        if(fragi+nhfrags<fragi_bot&&!frags[fragi+nhfrags].coded){
          loop_filter_v(ref+(ystride<<3),ystride,_bv);
        }
      }
      fragi++;
    }
    fragi0+=nhfrags;
  }
}

#if defined(OC_DUMP_IMAGES)
int32_t oc_state_dump_frame(const oc_theora_state *_state,int32_t _frame,
 const char *_suf){
  /*Dump a PNG of the reconstructed image.*/
  png_structp    png;
  png_infop      info;
  png_bytep     *image;
  FILE          *fp;
  char           fname[16];
  uchar *y_row;
  uchar *u_row;
  uchar *v_row;
  uchar *y;
  uchar *u;
  uchar *v;
  ogg_int64_t    iframe;
  ogg_int64_t    pframe;
  int32_t            y_stride;
  int32_t            u_stride;
  int32_t            v_stride;
  int32_t            framei;
  int32_t            width;
  int32_t            height;
  int32_t            imgi;
  int32_t            imgj;
  width=_state->info.frame_width;
  height=_state->info.frame_height;
  iframe=_state->granpos>>_state->info.keyframe_granule_shift;
  pframe=_state->granpos-(iframe<<_state->info.keyframe_granule_shift);
  sprintf(fname,"%08i%s.png",(int32_t)(iframe+pframe),_suf);
  fp=fopen(fname,"wb");
  if(fp==NULL)return TH_EFAULT;
  image=(png_bytep *)oc_malloc_2d(height,6*width,sizeof(**image));
  if(image==NULL){
    fclose(fp);
    return TH_EFAULT;
  }
  png=png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
  if(png==NULL){
    oc_free_2d(image);
    fclose(fp);
    return TH_EFAULT;
  }
  info=png_create_info_struct(png);
  if(info==NULL){
    png_destroy_write_struct(&png,NULL);
    oc_free_2d(image);
    fclose(fp);
    return TH_EFAULT;
  }
  if(setjmp(png_jmpbuf(png))){
    png_destroy_write_struct(&png,&info);
    oc_free_2d(image);
    fclose(fp);
    return TH_EFAULT;
  }
  framei=_state->ref_frame_idx[_frame];
  y_row=_state->ref_frame_bufs[framei][0].data;
  u_row=_state->ref_frame_bufs[framei][1].data;
  v_row=_state->ref_frame_bufs[framei][2].data;
  y_stride=_state->ref_frame_bufs[framei][0].stride;
  u_stride=_state->ref_frame_bufs[framei][1].stride;
  v_stride=_state->ref_frame_bufs[framei][2].stride;
  /*Chroma up-sampling is just done with a box filter.
    This is very likely what will actually be used in practice on a real
     display, and also removes one more layer to search in for the source of
     artifacts.
    As an added bonus, it's dead simple.*/
  for(imgi=height;imgi-->0;){
    int32_t dc;
    y=y_row;
    u=u_row;
    v=v_row;
    for(imgj=0;imgj<6*width;){
      float    yval;
      float    uval;
      float    vval;
      uint32_t rval;
      uint32_t gval;
      uint32_t bval;
      /*This is intentionally slow and very accurate.*/
      yval=(*y-16)*(1.0F/219);
      uval=(*u-128)*(2*(1-0.114F)/224);
      vval=(*v-128)*(2*(1-0.299F)/224);
      rval=OC_CLAMPI(0,(int32_t)(65535*(yval+vval)+0.5F),65535);
      gval=OC_CLAMPI(0,(int32_t)(65535*(
       yval-uval*(0.114F/0.587F)-vval*(0.299F/0.587F))+0.5F),65535);
      bval=OC_CLAMPI(0,(int32_t)(65535*(yval+uval)+0.5F),65535);
      image[imgi][imgj++]=(uchar)(rval>>8);
      image[imgi][imgj++]=(uchar)(rval&0xFF);
      image[imgi][imgj++]=(uchar)(gval>>8);
      image[imgi][imgj++]=(uchar)(gval&0xFF);
      image[imgi][imgj++]=(uchar)(bval>>8);
      image[imgi][imgj++]=(uchar)(bval&0xFF);
      dc=(y-y_row&1)|(_state->info.pixel_fmt&1);
      y++;
      u+=dc;
      v+=dc;
    }
    dc=-((height-1-imgi&1)|_state->info.pixel_fmt>>1);
    y_row+=y_stride;
    u_row+=dc&u_stride;
    v_row+=dc&v_stride;
  }
  png_init_io(png,fp);
  png_set_compression_level(png,Z_BEST_COMPRESSION);
  png_set_IHDR(png,info,width,height,16,PNG_COLOR_TYPE_RGB,
   PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
  switch(_state->info.colorspace){
    case TH_CS_ITU_REC_470M:{
      png_set_gAMA(png,info,2.2);
      png_set_cHRM_fixed(png,info,31006,31616,
       67000,32000,21000,71000,14000,8000);
    }break;
    case TH_CS_ITU_REC_470BG:{
      png_set_gAMA(png,info,2.67);
      png_set_cHRM_fixed(png,info,31271,32902,
       64000,33000,29000,60000,15000,6000);
    }break;
    default:break;
  }
  png_set_pHYs(png,info,_state->info.aspect_numerator,
   _state->info.aspect_denominator,0);
  png_set_rows(png,info,image);
  png_write_png(png,info,PNG_TRANSFORM_IDENTITY,NULL);
  png_write_end(png,info);
  png_destroy_write_struct(&png,&info);
  oc_free_2d(image);
  fclose(fp);
  return 0;
}
#endif



ogg_int64_t th_granule_frame(void *_encdec,ogg_int64_t _granpos){
  oc_theora_state *state;
  state=(oc_theora_state *)_encdec;
  if(_granpos>=0){
    ogg_int64_t iframe;
    ogg_int64_t pframe;
    iframe=_granpos>>state->info.keyframe_granule_shift;
    pframe=_granpos-(iframe<<state->info.keyframe_granule_shift);
    /*3.2.0 streams store the frame index in the granule position.
      3.2.1 and later store the frame count.
      We return the index, so adjust the value if we have a 3.2.1 or later
       stream.*/
    return iframe+pframe-TH_VERSION_CHECK(&state->info,3,2,1);
  }
  return -1;
}

double th_granule_time(void *_encdec,ogg_int64_t _granpos){
  oc_theora_state *state;
  state=(oc_theora_state *)_encdec;
  if(_granpos>=0){
    return (th_granule_frame(_encdec, _granpos)+1)*(
     (double)state->info.fps_denominator/state->info.fps_numerator);
  }
  return -1;
}
