#include <iostream>
#include <memory>
#include <vector>

#include <Windows.h>
#include <xaudio2.h>

#include <wil/result.h>
#include <wil/com.h>
#include <opusfile.h>

namespace N503::Audio::Format::Wave
{
#pragma pack(push)
#pragma pack(1)

    struct WaveFormatEx // msut be 18 bytes
    {
        std::uint16_t AudioFormat;
        std::uint16_t Channels;
        std::uint32_t SamplePerSecond;
        std::uint32_t BytesPerSecond;
        std::uint16_t BlockAlign;
        std::uint16_t BitsPerSample;
        std::uint16_t ExtraFormatSize;
    };

#pragma pack(pop)
}

int main()
{
    std::locale::global(std::locale(".UTF8"));

    // COMライブラリを初期化し必要に応じてスレッドの新しいアパートメントを作成します。
    auto&& CoUninitializeReservedCall = wil::CoInitializeEx();

    int error{};
    OggOpusFile* file = ::op_open_file(R"(G:\Develop\Assets\Audio\Opus\sample-2.opus)", &error);

    if (!file)
    {
        return 0;
    }

    wil::com_ptr<IXAudio2> xaudio2;
    IXAudio2MasteringVoice* pMasteringVoice{};
    IXAudio2SourceVoice*    pSourceVoice{};

    try
    {
        auto pOpusInfo = ::op_head(file, -1);

        if (!pOpusInfo)
        {
            throw std::runtime_error("op_head failed.");
        }

        N503::Audio::Format::Wave::WaveFormatEx waveFormat{};
        waveFormat.AudioFormat     = 1; // WAVE_FORMAT_PCM
        waveFormat.BitsPerSample   = 16;
        waveFormat.Channels        = pOpusInfo->channel_count;
        waveFormat.SamplePerSecond = 48000; // opusは48000固定
        waveFormat.BlockAlign      = waveFormat.BitsPerSample / 8 * waveFormat.Channels;
        waveFormat.BytesPerSecond  = waveFormat.SamplePerSecond * waveFormat.BlockAlign;
        waveFormat.ExtraFormatSize = 0;

        std::cout << "WaveFormat.AudioFormat     = " << waveFormat.AudioFormat     << std::endl;
        std::cout << "WaveFormat.BitsPerSample   = " << waveFormat.BitsPerSample   << std::endl;
        std::cout << "WaveFormat.Channels        = " << waveFormat.Channels        << std::endl;
        std::cout << "WaveFormat.SamplePerSecond = " << waveFormat.SamplePerSecond << std::endl;
        std::cout << "WaveFormat.BlockAlign      = " << waveFormat.BlockAlign      << std::endl;
        std::cout << "WaveFormat.BytesPerSecond  = " << waveFormat.BytesPerSecond  << std::endl;
        std::cout << "WaveFormat.ExtraFormatSize = " << waveFormat.ExtraFormatSize << std::endl;

        std::cout << "op_raw_total  = " << ::op_raw_total(file, -1)  << " bytes." << std::endl;
        std::cout << "op_pcm_total  = " << ::op_pcm_total(file, -1) * waveFormat.BlockAlign  << " bytes." << std::endl;
        //std::cout << "ov_time_total = " << ::ov_time_total(&file, -1) << " seconds." << std::endl;

        THROW_IF_FAILED(::XAudio2Create(xaudio2.put()));
        THROW_IF_FAILED(xaudio2->CreateMasteringVoice(&pMasteringVoice));
        THROW_IF_FAILED(xaudio2->CreateSourceVoice(&pSourceVoice, reinterpret_cast<WAVEFORMATEX*>(&waveFormat)));
        THROW_IF_FAILED(pSourceVoice->Start());

        XAUDIO2_BUFFER      xaudio2Buffer{};
        XAUDIO2_VOICE_STATE xaudio2VoiceState{};

        int bitStream{};

        std::vector<std::unique_ptr<std::int16_t[]>> buffers;
        buffers.emplace_back(std::make_unique<std::int16_t[]>(8192));
        buffers.emplace_back(std::make_unique<std::int16_t[]>(8192));
        buffers.emplace_back(std::make_unique<std::int16_t[]>(8192));
        buffers.emplace_back(std::make_unique<std::int16_t[]>(8192));
        buffers.emplace_back(std::make_unique<std::int16_t[]>(8192));

        int buffersIndex = 0;

        while (true)
        {
            if (pSourceVoice->GetState(&xaudio2VoiceState), xaudio2VoiceState.BuffersQueued >= buffers.size())
            {
                ::Sleep(1);
                continue;
            }

            auto result = ::op_read(file, buffers.at(buffersIndex).get(), 8192, nullptr);

            if (result <= 0)
            {
                std::cout << "End of stream. (length = " << result << ")" << std::endl;
                break;
            }

            xaudio2Buffer.Flags      = 0;
            xaudio2Buffer.AudioBytes = result * waveFormat.BlockAlign;
            xaudio2Buffer.pAudioData = reinterpret_cast<BYTE const*>(buffers.at(buffersIndex).get());

            THROW_IF_FAILED(pSourceVoice->SubmitSourceBuffer(&xaudio2Buffer));

            buffersIndex = (++buffersIndex) % buffers.size();
        }

        while (pSourceVoice->GetState(&xaudio2VoiceState), xaudio2VoiceState.BuffersQueued > 0)
        {
            ::Sleep(1);
        }
    }
    catch (std::exception const& exception)
    {
        std::cerr << exception.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "unknown error." << std::endl;
    }

    if (pSourceVoice)
    {
        pSourceVoice->Stop();
        pSourceVoice->FlushSourceBuffers();
        pSourceVoice->DestroyVoice();
    }

    if (pMasteringVoice)
    {
        pMasteringVoice->DestroyVoice();
    }

    ::op_free(file);

    return 0;
}
