#
# Copyright (C) 2015 Cybernetica
#
# Research/Commercial License Usage
# Licensees holding a valid Research License or Commercial License
# for the Software may use this file according to the written
# agreement between you and Cybernetica.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 3.0 as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL included in the
# packaging of this file.  Please review the following information to
# ensure the GNU General Public License version 3.0 requirements will be
# met: http://www.gnu.org/copyleft/gpl-3.0.html.
#
# For further information, please contact us at sharemind@cyber.ee.
#

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(SharemindLibConsensusService VERSION 0.1.3 LANGUAGES C CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCMakeHelpers 1.6 REQUIRED)


# LibConsensusService:
FILE(GLOB_RECURSE SharemindLibConsensusService_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
ADD_CUSTOM_TARGET("include_SharemindLibConsensusService_headers_in_IDE"
                  SOURCES ${SharemindLibConsensusService_HEADERS})
INSTALL(FILES ${SharemindLibConsensusService_HEADERS}
        DESTINATION "include/sharemind"
        COMPONENT "dev")
SharemindAddInterfaceLibrary(LibConsensusService)
TARGET_INCLUDE_DIRECTORIES(LibConsensusService
    INTERFACE
        # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src> # TODO
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
    )
SharemindCreateCMakeFindFilesForTarget(LibConsensusService)


# Packaging:
SharemindSetupPackaging()
SharemindAddComponentPackage(dev
    NAME "libsharemind-consensusservice-dev"
    DESCRIPTION "Sharemind consensus service development headers"
    DEB_SECTION "libdevel"
    DEB_DEPENDS
        "libc6-dev (>= 2.19)"
        "libstdc++-dev"
)
SharemindPackagingFinalize()
