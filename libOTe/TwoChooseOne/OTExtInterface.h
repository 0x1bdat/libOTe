#pragma once
// This file and the associated implementation has been placed in the public domain, waiving all copyright. No restrictions are placed on its use. 
#include <cryptoTools/Common/Defines.h>
#include <cryptoTools/Network/Channel.h>
#include <array>
#include <coproto/Proto.h>

#ifdef GetMessage
#undef GetMessage
#endif

namespace osuCrypto
{
    class PRNG;
#ifdef ENABLE_BOOST
    class Channel;
#endif
    class BitVector;

    // The hard coded number of base OT that is expected by the OT Extension implementations.
    // This can be changed if the code is adequately adapted. 
    const u64 gOtExtBaseOtCount(128);
    
    class OtReceiver
    {
    public:
        OtReceiver() {}


        // Receive random strings indexed by choices. The random strings will be written to 
        // messages.
        virtual coproto::Proto receive(
            const BitVector& choices,
            span<block> messages,
            PRNG& prng) = 0;


        // Receive chosen strings indexed by choices. The chosen strings will be written to 
        // messages.
        coproto::Proto receiveChosen(
            const BitVector& choices,
            span<block> recvMessages,
            PRNG& prng);

        coproto::Proto receiveCorrelated(
            const BitVector& choices,
            span<block> recvMessages,
            PRNG& prng);

#ifdef ENABLE_BOOST
        void receive(
            const BitVector& choices,
            span<block> messages,
            PRNG& prng,
            Channel& chl);

        // Receive chosen strings indexed by choices. The chosen strings will be written to 
        // messages.
        void receiveChosen(
            const BitVector& choices,
            span<block> recvMessages,
            PRNG& prng,
            Channel& chl);

        void receiveCorrelated(
            const BitVector& choices,
            span<block> recvMessages,
            PRNG& prng,
            Channel& chl);
#endif

    };

    class OtSender
    {
    public:
        OtSender() {}

        // send random strings. The random strings will be written to 
        // messages.
        virtual coproto::Proto send(
            span<std::array<block, 2>> messages,
            PRNG& prng) = 0;


        // send chosen strings. Thosen strings are read from messages.
        coproto::Proto sendChosen(
            span<std::array<block, 2>> messages,
            PRNG& prng);

        coproto::Proto sendCorrelated(
            span<block> messages, 
            std::function<block(block, u64)> corFunc, 
            PRNG& prng);
        

#ifdef ENABLE_BOOST

        void send(
            span<std::array<block, 2>> messages,
            PRNG& prng,
            Channel& chl);

        // send chosen strings. Thosen strings are read from messages.
        void sendChosen(
            span<std::array<block, 2>> messages,
            PRNG& prng,
            Channel& chl);

        template<typename CorrelationFunc>
        void sendCorrelated(span<block> messages, const CorrelationFunc& corFunc, PRNG& prng, Channel& chl)
        {

            std::vector<std::array<block, 2>> temp(messages.size());
            std::vector<block> temp2(messages.size());
            send(temp, prng, chl);

            for (u64 i = 0; i < static_cast<u64>(messages.size()); ++i)
            {
                messages[i] = temp[i][0];
                temp2[i] = temp[i][1] ^ corFunc(temp[i][0], i);
            }

            chl.asyncSend(std::move(temp2));
        }
#endif

    };


    class OtExtReceiver : public OtReceiver
    {
    public:
        OtExtReceiver() {}


        virtual void setUniformBaseOts(
            span<std::array<block, 2>> baseSendOts) = 0;

#ifdef ENABLE_BOOST
        // sets the base OTs that are then used to extend
        void setBaseOts(
            span<std::array<block,2>> baseSendOts,
            PRNG& prng,
            Channel& chl);

        void genBaseOts(PRNG& prng, Channel& chl);
#endif
        
        virtual coproto::Proto setBaseOts(
            span<std::array<block, 2>> baseSendOts,
            PRNG& prng);

        // the number of base OTs that should be set.
        virtual u64 baseOtCount() const { return gOtExtBaseOtCount; }

        // returns true if the base OTs are currently set.
        virtual bool hasBaseOts() const = 0; 
        
        // Returns an indpendent copy of this extender.
        virtual std::unique_ptr<OtExtReceiver> split() = 0;

        // use the default base OT class to generate the
        // base OTs that are required.
        virtual coproto::Proto genBaseOts(PRNG& prng);
    };

    class OtExtSender : public OtSender
    {
    public:
        OtExtSender() {}

        // the number of base OTs that should be set.
        virtual u64 baseOtCount() const { return gOtExtBaseOtCount; }

        // returns true if the base OTs are currently set.
        virtual bool hasBaseOts() const = 0;

        virtual  void setUniformBaseOts(
            span<block> baseRecvOts,
            const BitVector& choices) = 0;

        // sets the base OTs that are then used to extend
        virtual coproto::Proto setBaseOts(
            span<block> baseRecvOts,
            const BitVector& choices);

        // Returns an indpendent copy of this extender.
        virtual std::unique_ptr<OtExtSender> split() = 0;

        // use the default base OT class to generate the
        // base OTs that are required.
        virtual coproto::Proto genBaseOts(PRNG& prng);

#ifdef ENABLE_BOOST
        void setBaseOts(
            span<block> baseRecvOts,
            const BitVector& choices,
            Channel& chl);

        void genBaseOts(PRNG& prng, Channel& chl);
#endif

    };


}
