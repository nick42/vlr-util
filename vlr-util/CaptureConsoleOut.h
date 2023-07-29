#pragma once

// Note: Code inspired from: https://github.com/dmikushin/stdcapture/blob/master/include/stdcapture/stdcapture.h
// (That source licensed under BSD3)

#ifdef _MSC_VER
#include <io.h>
#define popen _popen 
#define pclose _pclose
#define stat _stat 
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#define close _close
#define pipe _pipe
#define read _read
#define eof _eof
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <mutex>
#include <functional>

#ifndef STD_OUT_FD 
#define STD_OUT_FD (fileno(stdout)) 
#endif 

#ifndef STD_ERR_FD 
#define STD_ERR_FD (fileno(stderr)) 
#endif

#include "util.Result.h"

VLR_NAMESPACE_BEGIN(vlr)

VLR_NAMESPACE_BEGIN(util)

VLR_NAMESPACE_BEGIN(detail)

int secure_dup(int src);
void secure_pipe(int* pipes);
void secure_dup2(int src, int dest);
void secure_close(int& fd);

class CCapturePipeData
{
public:
    // Note: For each pipe we capture, we potentially create two replacement pipes, one for each direction
    struct PipeDirection
    {
        enum EPipeDirection
        {
            Read,
            Write,
        };
    };

    FILE* m_pStreamForCapture = {};
    int m_nFileDescriptorForCapture = {};

    bool m_bCaptureInProgress = false;
    int m_arrDupPipeHandles[2] = {};
    int m_nPreviousPipeHandle = {};

    using FOnCaptureData = std::function<void(const char*, size_t)>;
    FOnCaptureData m_fOnCaptureData;
    std::vector<char> m_arrInternalDataCapture;

public:
    struct CaptureOptions
    {
        FILE* m_pStream = {};
        int m_nFileDescriptorToCapture = {};
        // Note: If this is not set, capture will default to internal
        FOnCaptureData m_fOnCaptureData;

        decltype(auto) withStream(FILE* pStream)
        {
            m_pStream = pStream;
            return *this;
        }
        decltype(auto) withFileDescriptorToCapture(int nFileDescriptorToCapture)
        {
            m_nFileDescriptorToCapture = nFileDescriptorToCapture;
            return *this;
        }
        template< typename TCallback >
        decltype(auto) withOnDataCapture(const TCallback& tCallback)
        {
            m_fOnCaptureData = tCallback;
            return *this;
        }
    };
    StandardResult BeginCapture(const CaptureOptions& oCaptureOptions);
    inline StandardResult BeginCapture(FILE* pStream, int nFileDescriptorToCapture)
    {
        return BeginCapture(CaptureOptions{}.withStream(pStream).withFileDescriptorToCapture(nFileDescriptorToCapture));
    }
    StandardResult EndCapture();
    inline bool IsCaptureInProgress() const
    {
        return m_bCaptureInProgress;
    }
};

VLR_NAMESPACE_END //(detail)

class CCaptureConsoleDataAnalysisHelper
{
public:
    std::shared_ptr<detail::CCapturePipeData> m_spCapturePipeData;
    std::vector<std::string_view> m_arrTextLines;

public:
    const std::vector<std::string_view>& GetAsTextLines();

    int FindMatchingLineIndex_Contains(std::string_view svLine, int nStartIndex = 0);

public:
    CCaptureConsoleDataAnalysisHelper(const std::shared_ptr<detail::CCapturePipeData>& spCapturePipeData)
        : m_spCapturePipeData{ spCapturePipeData }
    {}
};

class CCaptureConsoleOut
{
protected:
    std::shared_ptr<detail::CCapturePipeData> m_spCapturePipeData_StdOut;
    std::shared_ptr<detail::CCapturePipeData> m_spCapturePipeData_StdErr;

public:
    struct CaptureOptions
    {
        bool m_bCaptureStdOut = true;
        bool m_bCaptureStdErr = true;

        static constexpr CaptureOptions CaptureAll()
        {
            return []()
            {
                auto oCaptureOptions = CaptureOptions{};
                oCaptureOptions.m_bCaptureStdOut = true;
                oCaptureOptions.m_bCaptureStdOut = true;
                return oCaptureOptions;
            }();
        }
    };
    StandardResult BeginCapture(const CaptureOptions& oCaptureOptions = CaptureOptions::CaptureAll());
    StandardResult EndCapture();
    inline bool IsCaptureInProgress() const
    {
        auto bCaptureInProgress = false;
        if (m_spCapturePipeData_StdOut && m_spCapturePipeData_StdOut->IsCaptureInProgress())
        {
            bCaptureInProgress = true;
        }
        if (m_spCapturePipeData_StdErr && m_spCapturePipeData_StdErr->IsCaptureInProgress())
        {
            bCaptureInProgress = true;
        }

        return bCaptureInProgress;
    }

    auto GetDataAnalysisHelper_StdOut() const
    {
        return CCaptureConsoleDataAnalysisHelper{ m_spCapturePipeData_StdOut };
    }
    auto GetDataAnalysisHelper_StdErr() const
    {
        return CCaptureConsoleDataAnalysisHelper{ m_spCapturePipeData_StdErr };
    }

    class CScopedConsoleDataCapture
    {
    protected:
        CCaptureConsoleOut& m_oCaptureConsoleOutput;

    public:
        enum ECaptureFlags
        {
            CaptureStdOut = 1 << 0,
            CaptureStdErr = 1 << 1,
        };
        static constexpr DWORD m_dwCaptureFlagsAll = CaptureStdOut | CaptureStdErr;

    protected:
        StandardResult DoConstructor(DWORD nCaptureFlags);
        StandardResult DoDestructor();
    public:
        CScopedConsoleDataCapture(CCaptureConsoleOut& oCaptureConsoleOutput, DWORD nCaptureFlags = m_dwCaptureFlagsAll)
            : m_oCaptureConsoleOutput{ oCaptureConsoleOutput }
        {
            DoConstructor(nCaptureFlags);
        }
        CScopedConsoleDataCapture(const CScopedConsoleDataCapture&) = delete;
        CScopedConsoleDataCapture& operator=(const CScopedConsoleDataCapture&) = delete;
        ~CScopedConsoleDataCapture()
        {
            DoDestructor();
        }
    };

    auto GetScopedCapture(DWORD nCaptureFlags = CScopedConsoleDataCapture::m_dwCaptureFlagsAll)
    {
        return CScopedConsoleDataCapture(*this, nCaptureFlags);
    }

protected:
    StandardResult DoConstructor();
    StandardResult DoDestructor();
public:
    CCaptureConsoleOut()
    {
        DoConstructor();
    }
    ~CCaptureConsoleOut()
    {
        DoDestructor();
    }
};

VLR_NAMESPACE_END //(util)

VLR_NAMESPACE_END //(vlr)
