#include "pch.h"
#include "CaptureConsoleOut.h"

#include <chrono>
#include <thread>

#include "strings.split.h"

VLR_NAMESPACE_BEGIN(vlr)

VLR_NAMESPACE_BEGIN(util)

VLR_NAMESPACE_BEGIN(detail)

int secure_dup(int src)
{
	int ret = -1;
	bool fd_blocked = false;
	do
	{
		ret = dup(src);
		fd_blocked = (errno == EINTR || errno == EBUSY);
		if (fd_blocked)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	} while (ret < 0);
	return ret;
}

void secure_pipe(int* pipes)
{
	int ret = -1;
	bool fd_blocked = false;
	do
	{
#ifdef _MSC_VER
		ret = pipe(pipes, 65536, O_BINARY);
#else
		ret = pipe(pipes) == -1;
#endif
		fd_blocked = (errno == EINTR || errno == EBUSY);
		if (fd_blocked)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	} while (ret < 0);
}

void secure_dup2(int src, int dest)
{
	int ret = -1;
	bool fd_blocked = false;
	do
	{
		ret = dup2(src, dest);
		fd_blocked = (errno == EINTR || errno == EBUSY);
		if (fd_blocked)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	} while (ret < 0);
}

void secure_close(int& fd)
{
	int ret = -1;
	bool fd_blocked = false;
	do
	{
		ret = close(fd);
		fd_blocked = (errno == EINTR);
		if (fd_blocked)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	} while (ret < 0);

	fd = -1;
}

StandardResult CCapturePipeData::BeginCapture(const CaptureOptions& oCaptureOptions)
{
    m_pStreamForCapture = oCaptureOptions.m_pStream;
    m_nFileDescriptorForCapture = oCaptureOptions.m_nFileDescriptorToCapture;

    // Make output stream unbuffered, so that we don't need to flush
    // the streams before capture and after capture (fflush can cause a deadlock)
    setvbuf(m_pStreamForCapture, NULL, _IONBF, 0);

    // Start capturing.
    secure_pipe(m_arrDupPipeHandles);
    m_nPreviousPipeHandle = secure_dup(m_nFileDescriptorForCapture);
    secure_dup2(m_arrDupPipeHandles[PipeDirection::Write], m_nFileDescriptorForCapture);
#ifndef _MSC_VER
    secure_close(m_arrDupPipeHandles[PipeDirection::Write]);
#endif

    // If the user has specified a custom callback for data, use that, else map to internal storage
    if (oCaptureOptions.m_fOnCaptureData)
    {
        m_fOnCaptureData = oCaptureOptions.m_fOnCaptureData;
    }
    else
    {
        m_fOnCaptureData = [this](const char* data, size_t size)
        {
            this->m_arrInternalDataCapture.reserve(this->m_arrInternalDataCapture.size() + size);
            for (size_t i = 0; i < size; ++i)
            {
                this->m_arrInternalDataCapture.push_back(data[i]);
            }
        };
    }

    return StandardResult::Success;
}

StandardResult CCapturePipeData::EndCapture()
{
	// End capturing.
	secure_dup2(m_nPreviousPipeHandle, m_nFileDescriptorForCapture);

	const int bufSize = 4096;
	char buf[bufSize];
	int bytesRead = 0;
	bool fd_blocked(false);
	do
	{
		bytesRead = 0;
		fd_blocked = false;
#ifdef _MSC_VER
		if (!eof(m_arrDupPipeHandles[PipeDirection::Read]))
			bytesRead = read(m_arrDupPipeHandles[PipeDirection::Read], buf, bufSize - 1);
#else
		bytesRead = read(m_arrDupPipeHandles[PipeDirection::Read], buf, bufSize - 1);
#endif
		if (bytesRead > 0)
		{
			buf[bytesRead] = 0;
            m_fOnCaptureData(buf, bytesRead);
		}
		else if (bytesRead < 0)
		{
			fd_blocked = (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR);
			if (fd_blocked)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	} while (fd_blocked || bytesRead == (bufSize - 1));

	secure_close(m_nPreviousPipeHandle);
	secure_close(m_arrDupPipeHandles[PipeDirection::Read]);
#ifdef _MSC_VER
	secure_close(m_arrDupPipeHandles[PipeDirection::Write]);
#endif

	return StandardResult::Success;
}

VLR_NAMESPACE_END //(detail)

const std::vector<std::string_view>& CCaptureConsoleDataAnalysisHelper::GetAsTextLines()
{
	if (m_arrTextLines.size() > 0)
	{
		return m_arrTextLines;
	}
	auto& arrDataBuffer = m_spCapturePipeData->m_arrInternalDataCapture;
	if (arrDataBuffer.size() == 0)
	{
		return m_arrTextLines;
	}
	auto svDataBuffer = std::string_view{ arrDataBuffer.data(), arrDataBuffer.size() };

	strings::SplitStringAtDelimiter(svDataBuffer, '\r\n', m_arrTextLines);

	return m_arrTextLines;
}

int CCaptureConsoleDataAnalysisHelper::FindMatchingLineIndex_Contains(std::string_view svFindData, int nStartIndex /*= 0*/)
{
	auto& arrTextLines = GetAsTextLines();
	auto nCurrentIndex = nStartIndex;
	while (nCurrentIndex < arrTextLines.size())
	{
		auto svCurrentLine = arrTextLines[nCurrentIndex++];
		if (svCurrentLine.find(svFindData) == std::string_view::npos)
		{
			continue;
		}
		return nCurrentIndex - 1;
	}

	return -1;
}

StandardResult CCaptureConsoleOut::BeginCapture(const CaptureOptions& oCaptureOptions /*= {}*/)
{
	StandardResult sr;

	if (oCaptureOptions.m_bCaptureStdOut)
	{
		m_spCapturePipeData_StdOut = std::make_shared<detail::CCapturePipeData>();
		VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION(m_spCapturePipeData_StdOut, StandardResult::Failure);

		sr = m_spCapturePipeData_StdOut->BeginCapture(stdout, STD_OUT_FD);
		VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);
	}
	if (oCaptureOptions.m_bCaptureStdOut)
	{
		m_spCapturePipeData_StdErr = std::make_shared<detail::CCapturePipeData>();
		VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION(m_spCapturePipeData_StdErr, StandardResult::Failure);

		sr = m_spCapturePipeData_StdErr->BeginCapture(stderr, STD_ERR_FD);
		VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);
	}

	return StandardResult::Success;
}

StandardResult CCaptureConsoleOut::EndCapture()
{
	StandardResult sr;

	if (m_spCapturePipeData_StdOut)
	{
		sr = m_spCapturePipeData_StdOut->EndCapture();
		VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);
	}
	if (m_spCapturePipeData_StdErr)
	{
		sr = m_spCapturePipeData_StdErr->EndCapture();
		VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);
	}

	return StandardResult::Success;
}

StandardResult CCaptureConsoleOut::DoConstructor()
{
    return StandardResult::Success;
}

StandardResult CCaptureConsoleOut::DoDestructor()
{
    StandardResult sr;

    if (IsCaptureInProgress())
    {
        sr = EndCapture();
        VLR_ASSERT_SR_SUCCEEDED_OR_CONTINUE(sr);
    }

    return StandardResult::Success;
}

VLR_NAMESPACE_END //(util)

VLR_NAMESPACE_END //(vlr)
