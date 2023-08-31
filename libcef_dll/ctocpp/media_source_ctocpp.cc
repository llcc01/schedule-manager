// Copyright (c) 2023 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=4ef592680d7650fb636a997f127d3c8e54d6c450$
//

#include "libcef_dll/ctocpp/media_source_ctocpp.h"
#include "libcef_dll/shutdown_checker.h"

// VIRTUAL METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall") CefString CefMediaSourceCToCpp::GetId() {
  shutdown_checker::AssertNotShutdown();

  cef_media_source_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_id)) {
    return CefString();
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->get_id(_struct);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

NO_SANITIZE("cfi-icall") bool CefMediaSourceCToCpp::IsCastSource() {
  shutdown_checker::AssertNotShutdown();

  cef_media_source_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_cast_source)) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_cast_source(_struct);

  // Return type: bool
  return _retval ? true : false;
}

NO_SANITIZE("cfi-icall") bool CefMediaSourceCToCpp::IsDialSource() {
  shutdown_checker::AssertNotShutdown();

  cef_media_source_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_dial_source)) {
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_dial_source(_struct);

  // Return type: bool
  return _retval ? true : false;
}

// CONSTRUCTOR - Do not edit by hand.

CefMediaSourceCToCpp::CefMediaSourceCToCpp() {}

// DESTRUCTOR - Do not edit by hand.

CefMediaSourceCToCpp::~CefMediaSourceCToCpp() {
  shutdown_checker::AssertNotShutdown();
}

template <>
cef_media_source_t*
CefCToCppRefCounted<CefMediaSourceCToCpp, CefMediaSource, cef_media_source_t>::
    UnwrapDerived(CefWrapperType type, CefMediaSource* c) {
  NOTREACHED() << "Unexpected class type: " << type;
  return nullptr;
}

template <>
CefWrapperType CefCToCppRefCounted<CefMediaSourceCToCpp,
                                   CefMediaSource,
                                   cef_media_source_t>::kWrapperType =
    WT_MEDIA_SOURCE;