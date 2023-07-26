/*******************************************************************************************************************
Copyright (c) 2023 Cycling '74

The code that Max generates automatically and that end users are capable of
exporting and using, and any associated documentation files (the “Software”)
is a work of authorship for which Cycling '74 is the author and owner for
copyright purposes.

This Software is dual-licensed either under the terms of the Cycling '74
License for Max-Generated Code for Export, or alternatively under the terms
of the General Public License (GPL) Version 3. You may use the Software
according to either of these licenses as it is most appropriate for your
project on a case-by-case basis (proprietary or not).

A) Cycling '74 License for Max-Generated Code for Export

A license is hereby granted, free of charge, to any person obtaining a copy
of the Software (“Licensee”) to use, copy, modify, merge, publish, and
distribute copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The Software is licensed to Licensee for all uses that do not include the sale,
sublicensing, or commercial distribution of software that incorporates this
source code. This means that the Licensee is free to use this software for
educational, research, and prototyping purposes, to create musical or other
creative works with software that incorporates this source code, or any other
use that does not constitute selling software that makes use of this source
code. Commercial distribution also includes the packaging of free software with
other paid software, hardware, or software-provided commercial services.

For entities with UNDER $200k in annual revenue or funding, a license is hereby
granted, free of charge, for the sale, sublicensing, or commercial distribution
of software that incorporates this source code, for as long as the entity's
annual revenue remains below $200k annual revenue or funding.

For entities with OVER $200k in annual revenue or funding interested in the
sale, sublicensing, or commercial distribution of software that incorporates
this source code, please send inquiries to licensing@cycling74.com.

The above copyright notice and this license shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Please see
https://support.cycling74.com/hc/en-us/articles/10730637742483-RNBO-Export-Licensing-FAQ
for additional information

B) General Public License Version 3 (GPLv3)
Details of the GPLv3 license can be found at: https://www.gnu.org/licenses/gpl-3.0.html
*******************************************************************************************************************/

#include "RNBO_Common.h"
#include "RNBO_AudioSignal.h"

namespace RNBO {


#define floor(x) ((long)(x))

#if defined(__GNUC__) || defined(__clang__)
    #define RNBO_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define RNBO_RESTRICT __restrict
#endif

#define FIXEDSIZEARRAYINIT(...) { }

class rnbomatic : public PatcherInterfaceImpl {
public:

rnbomatic()
{
}

~rnbomatic()
{
}

rnbomatic* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
    getEngine()->flushClockEvents(this, 1935387534, false);
    getEngine()->flushClockEvents(this, -1508480176, false);
    getEngine()->flushClockEvents(this, 922084457, false);
    getEngine()->flushClockEvents(this, 1937376702, false);
    getEngine()->flushClockEvents(this, -4141894, false);
}

template <typename T> void listquicksort(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    if (l < h) {
        Int p = (Int)(this->listpartition(arr, sortindices, l, h, ascending));
        this->listquicksort(arr, sortindices, l, p - 1, ascending);
        this->listquicksort(arr, sortindices, p + 1, h, ascending);
    }
}

template <typename T> Int listpartition(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    number x = arr[(Index)h];
    Int i = (Int)(l - 1);

    for (Int j = (Int)(l); j <= h - 1; j++) {
        bool asc = (bool)((bool)(ascending) && arr[(Index)j] <= x);
        bool desc = (bool)((bool)(!(bool)(ascending)) && arr[(Index)j] >= x);

        if ((bool)(asc) || (bool)(desc)) {
            i++;
            this->listswapelements(arr, i, j);
            this->listswapelements(sortindices, i, j);
        }
    }

    i++;
    this->listswapelements(arr, i, h);
    this->listswapelements(sortindices, i, h);
    return i;
}

template <typename T> void listswapelements(T& arr, Int a, Int b) {
    auto tmp = arr[(Index)a];
    arr[(Index)a] = arr[(Index)b];
    arr[(Index)b] = tmp;
}

inline number safediv(number num, number denom) {
    return (denom == 0.0 ? 0.0 : num / denom);
}

number safepow(number base, number exponent) {
    return fixnan(rnbo_pow(base, exponent));
}

number samplerate() {
    return this->sr;
}

SampleIndex currentsampletime() {
    return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
}

number mstosamps(MillisecondTime ms) {
    return ms * this->sr * 0.001;
}

number maximum(number x, number y) {
    return (x < y ? y : x);
}

MillisecondTime currenttime() {
    return this->_currentTime;
}

number tempo() {
    return this->getTopLevelPatcher()->globaltransport_getTempo(this->currenttime());
}

number mstobeats(number ms) {
    return ms * this->tempo() * 0.008 / (number)480;
}

MillisecondTime sampstoms(number samps) {
    return samps * 1000 / this->sr;
}

Index getNumMidiInputPorts() const {
    return 0;
}

void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}

Index getNumMidiOutputPorts() const {
    return 0;
}

void process(
    const SampleValue * const* inputs,
    Index numInputs,
    SampleValue * const* outputs,
    Index numOutputs,
    Index n
) {
    this->vs = n;
    this->updateTime(this->getEngine()->getCurrentTime());
    SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
    SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
    const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);

    this->cycle_tilde_01_perform(
        this->cycle_tilde_01_frequency,
        this->cycle_tilde_01_phase_offset,
        this->signals[0],
        this->dummyBuffer,
        n
    );

    this->cycle_tilde_02_perform(
        this->cycle_tilde_02_frequency,
        this->cycle_tilde_02_phase_offset,
        this->signals[1],
        this->dummyBuffer,
        n
    );

    this->scale_tilde_01_perform(
        this->signals[1],
        this->scale_tilde_01_lowin,
        this->scale_tilde_01_hiin,
        this->scale_tilde_01_lowout,
        this->scale_tilde_01_highout,
        this->scale_tilde_01_pow,
        this->signals[2],
        n
    );

    this->scale_tilde_02_perform(
        this->signals[1],
        this->scale_tilde_02_lowin,
        this->scale_tilde_02_hiin,
        this->scale_tilde_02_lowout,
        this->scale_tilde_02_highout,
        this->scale_tilde_02_pow,
        this->signals[3],
        n
    );

    this->line_01_perform(this->signals[1], n);
    this->dspexpr_06_perform(this->signals[1], this->signals[4], n);
    this->dspexpr_05_perform(this->signals[0], this->signals[4], this->signals[1], n);
    this->signaladder_01_perform(in2, in1, this->signals[1], this->signals[0], n);
    this->dspexpr_04_perform(this->signals[0], this->signals[3], this->signals[4], n);
    this->signaladder_02_perform(in2, in1, this->signals[1], this->signals[1], n);
    this->dspexpr_02_perform(this->signals[1], this->signals[2], this->signals[3], n);
    this->line_02_perform(this->signals[2], n);
    this->dspexpr_03_perform(this->signals[4], this->signals[2], out2, n);
    this->dspexpr_01_perform(this->signals[3], this->signals[2], out1, n);
    this->ip_01_perform(this->signals[2], n);
    this->dspexpr_07_perform(this->signals[2], this->dspexpr_07_in2, this->signals[3], n);
    this->phasor_01_perform(this->signals[3], this->signals[2], n);
    this->snapshot_01_perform(this->signals[2], n);
    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        Index i;

        for (i = 0; i < 5; i++) {
            this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
        }

        this->ip_01_sigbuf = resizeSignal(this->ip_01_sigbuf, this->maxvs, maxBlockSize);
        this->phasor_01_sigbuf = resizeSignal(this->phasor_01_sigbuf, this->maxvs, maxBlockSize);
        this->globaltransport_tempo = resizeSignal(this->globaltransport_tempo, this->maxvs, maxBlockSize);
        this->globaltransport_state = resizeSignal(this->globaltransport_state, this->maxvs, maxBlockSize);
        this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
        this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
        this->didAllocateSignals = true;
    }

    const bool sampleRateChanged = sampleRate != this->sr;
    const bool maxvsChanged = maxBlockSize != this->maxvs;
    const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;

    if (sampleRateChanged || maxvsChanged) {
        this->vs = maxBlockSize;
        this->maxvs = maxBlockSize;
        this->sr = sampleRate;
        this->invsr = 1 / sampleRate;
    }

    this->cycle_tilde_01_dspsetup(forceDSPSetup);
    this->cycle_tilde_02_dspsetup(forceDSPSetup);
    this->ip_01_dspsetup(forceDSPSetup);
    this->phasor_01_dspsetup(forceDSPSetup);
    this->globaltransport_dspsetup(forceDSPSetup);

    if (sampleRateChanged)
        this->onSampleRateChanged(sampleRate);
}

void setProbingTarget(MessageTag id) {
    switch (id) {
    default:
        {
        this->setProbingIndex(-1);
        break;
        }
    }
}

void setProbingIndex(ProbingIndex ) {}

Index getProbingChannels(MessageTag outletId) const {
    RNBO_UNUSED(outletId);
    return 0;
}

DataRef* getDataRef(DataRefIndex index)  {
    switch (index) {
    case 0:
        {
        return addressOf(this->RNBODefaultSinus);
        break;
        }
    default:
        {
        return nullptr;
        }
    }
}

DataRefIndex getNumDataRefs() const {
    return 1;
}

void fillRNBODefaultSinus(DataRef& ref) {
    Float64BufferRef buffer;
    buffer = new Float64Buffer(ref);
    number bufsize = buffer->getSize();

    for (Index i = 0; i < bufsize; i++) {
        buffer[i] = rnbo_cos(i * 3.14159265358979323846 * 2. / bufsize);
    }
}

void fillDataRef(DataRefIndex index, DataRef& ref) {
    switch (index) {
    case 0:
        {
        this->fillRNBODefaultSinus(ref);
        break;
        }
    }
}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    this->updateTime(time);

    if (index == 0) {
        this->cycle_tilde_01_buffer = new Float64Buffer(this->RNBODefaultSinus);
        this->cycle_tilde_01_bufferUpdated();
        this->cycle_tilde_02_buffer = new Float64Buffer(this->RNBODefaultSinus);
        this->cycle_tilde_02_bufferUpdated();
    }
}

void initialize() {
    this->RNBODefaultSinus = initDataRef("RNBODefaultSinus", true, nullptr, "buffer~");
    this->assign_defaults();
    this->setState();
    this->RNBODefaultSinus->setIndex(0);
    this->cycle_tilde_01_buffer = new Float64Buffer(this->RNBODefaultSinus);
    this->cycle_tilde_02_buffer = new Float64Buffer(this->RNBODefaultSinus);
    this->initializeObjects();
    this->allocateDataRefs();
    this->startup();
}

Index getIsMuted()  {
    return this->isMuted;
}

void setIsMuted(Index v)  {
    this->isMuted = v;
}

Index getPatcherSerial() const {
    return 0;
}

void getState(PatcherStateInterface& ) {}

void setState() {}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->param_01_getPresetValue(getSubState(preset, "volume"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->param_01_setPresetValue(getSubState(preset, "volume"));
}

void processTempoEvent(MillisecondTime time, Tempo tempo) {
    this->updateTime(time);

    if (this->globaltransport_setTempo(this->_currentTime, tempo, false))
        {}
}

void processTransportEvent(MillisecondTime time, TransportState state) {
    this->updateTime(time);

    if (this->globaltransport_setState(this->_currentTime, state, false)) {
        this->metro_01_onTransportChanged(state);
    }
}

void processBeatTimeEvent(MillisecondTime time, BeatTime beattime) {
    this->updateTime(time);

    if (this->globaltransport_setBeatTime(this->_currentTime, beattime, false)) {
        this->metro_01_onBeatTimeChanged(beattime);
    }
}

void onSampleRateChanged(double ) {}

void processTimeSignatureEvent(MillisecondTime time, int numerator, int denominator) {
    this->updateTime(time);

    if (this->globaltransport_setTimeSignature(this->_currentTime, numerator, denominator, false))
        {}
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    this->updateTime(time);

    switch (index) {
    case 0:
        {
        this->param_01_value_set(v);
        break;
        }
    }
}

void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValue(index, value, time);
}

void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValueNormalized(index, value, time);
}

ParameterValue getParameterValue(ParameterIndex index)  {
    switch (index) {
    case 0:
        {
        return this->param_01_value;
        }
    default:
        {
        return 0;
        }
    }
}

ParameterIndex getNumSignalInParameters() const {
    return 0;
}

ParameterIndex getNumSignalOutParameters() const {
    return 0;
}

ParameterIndex getNumParameters() const {
    return 1;
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "volume";
        }
    default:
        {
        return "bogus";
        }
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "volume";
        }
    default:
        {
        return "bogus";
        }
    }
}

void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
    {
        switch (index) {
        case 0:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = -70;
            info->min = -70;
            info->max = 0;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
    if (steps == 1) {
        if (normalizedValue > 0) {
            normalizedValue = 1.;
        }
    } else {
        ParameterValue oneStep = (number)1. / (steps - 1);
        ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
        normalizedValue = numberOfSteps * oneStep;
    }

    return normalizedValue;
}

ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 0:
        {
        {
            value = (value < -70 ? -70 : (value > 0 ? 0 : value));
            ParameterValue normalizedValue = (value - -70) / (0 - -70);
            return normalizedValue;
        }
        }
    default:
        {
        return value;
        }
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 0:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return -70 + value * (0 - -70);
            }
        }
        }
    default:
        {
        return value;
        }
    }
}

ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 0:
        {
        return this->param_01_value_constrain(value);
        }
    default:
        {
        return value;
        }
    }
}

void scheduleParamInit(ParameterIndex index, Index order) {
    this->paramInitIndices->push(index);
    this->paramInitOrder->push(order);
}

void processParamInitEvents() {
    this->listquicksort(
        this->paramInitOrder,
        this->paramInitIndices,
        0,
        (int)(this->paramInitOrder->length - 1),
        true
    );

    for (Index i = 0; i < this->paramInitOrder->length; i++) {
        this->getEngine()->scheduleParameterChange(
            this->paramInitIndices[i],
            this->getParameterValue(this->paramInitIndices[i]),
            0
        );
    }
}

void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
    RNBO_UNUSED(hasValue);
    this->updateTime(time);

    switch (index) {
    case 1935387534:
        {
        this->metro_01_tick_bang();
        break;
        }
    case -1508480176:
        {
        this->snapshot_01_out_set(value);
        break;
        }
    case 922084457:
        {
        this->delay_01_out_bang();
        break;
        }
    case 1937376702:
        {
        this->line_01_target_bang();
        break;
        }
    case -4141894:
        {
        this->line_02_target_bang();
        break;
        }
    }
}

void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}

void processOutletEvent(
    EngineLink* sender,
    OutletIndex index,
    ParameterValue value,
    MillisecondTime time
) {
    this->updateTime(time);
    this->processOutletAtCurrentTime(sender, index, value);
}

void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
    this->updateTime(time);

    switch (tag) {
    case TAG("valin"):
        {
        if (TAG("toggle_obj-36") == objectId)
            this->toggle_01_valin_set(payload);

        if (TAG("number_obj-80") == objectId)
            this->numberobj_01_valin_set(payload);

        if (TAG("toggle_obj-73") == objectId)
            this->toggle_02_valin_set(payload);

        if (TAG("number_obj-59") == objectId)
            this->numberobj_02_valin_set(payload);

        break;
        }
    case TAG("state"):
        {
        this->inport_01_value_number_set(payload);
        break;
        }
    case TAG("format"):
        {
        if (TAG("number_obj-80") == objectId)
            this->numberobj_01_format_set(payload);

        if (TAG("number_obj-59") == objectId)
            this->numberobj_02_format_set(payload);

        break;
        }
    case TAG("listin"):
        {
        if (TAG("message_obj-53") == objectId)
            this->message_01_listin_number_set(payload);

        if (TAG("message_obj-65") == objectId)
            this->message_02_listin_number_set(payload);

        if (TAG("message_obj-56") == objectId)
            this->message_03_listin_number_set(payload);

        break;
        }
    }
}

void processListMessage(
    MessageTag tag,
    MessageTag objectId,
    MillisecondTime time,
    const list& payload
) {
    this->updateTime(time);

    switch (tag) {
    case TAG("state"):
        {
        this->inport_01_value_list_set(payload);
        break;
        }
    case TAG("listin"):
        {
        if (TAG("message_obj-53") == objectId)
            this->message_01_listin_list_set(payload);

        if (TAG("message_obj-65") == objectId)
            this->message_02_listin_list_set(payload);

        if (TAG("message_obj-56") == objectId)
            this->message_03_listin_list_set(payload);

        break;
        }
    }
}

void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
    this->updateTime(time);

    switch (tag) {
    case TAG("state"):
        {
        this->inport_01_value_bang_bang();
        break;
        }
    case TAG("listin"):
        {
        if (TAG("message_obj-53") == objectId)
            this->message_01_listin_bang_bang();

        if (TAG("message_obj-65") == objectId)
            this->message_02_listin_bang_bang();

        if (TAG("message_obj-56") == objectId)
            this->message_03_listin_bang_bang();

        break;
        }
    }
}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {
    case TAG("valout"):
        {
        return "valout";
        }
    case TAG("toggle_obj-36"):
        {
        return "toggle_obj-36";
        }
    case TAG("number_obj-80"):
        {
        return "number_obj-80";
        }
    case TAG("setup"):
        {
        return "setup";
        }
    case TAG("toggle_obj-73"):
        {
        return "toggle_obj-73";
        }
    case TAG("time"):
        {
        return "time";
        }
    case TAG(""):
        {
        return "";
        }
    case TAG("number_obj-59"):
        {
        return "number_obj-59";
        }
    case TAG("listout"):
        {
        return "listout";
        }
    case TAG("message_obj-53"):
        {
        return "message_obj-53";
        }
    case TAG("message_obj-65"):
        {
        return "message_obj-65";
        }
    case TAG("message_obj-56"):
        {
        return "message_obj-56";
        }
    case TAG("valin"):
        {
        return "valin";
        }
    case TAG("state"):
        {
        return "state";
        }
    case TAG("format"):
        {
        return "format";
        }
    case TAG("listin"):
        {
        return "listin";
        }
    }

    return "";
}

MessageIndex getNumMessages() const {
    return 2;
}

const MessageInfo& getMessageInfo(MessageIndex index) const {
    switch (index) {
    case 0:
        {
        static const MessageInfo r0 = {
            "time",
            Outport
        };

        return r0;
        }
    case 1:
        {
        static const MessageInfo r1 = {
            "state",
            Inport
        };

        return r1;
        }
    }

    return NullMessageInfo;
}

protected:

void param_01_value_set(number v) {
    v = this->param_01_value_constrain(v);
    this->param_01_value = v;
    this->sendParameter(0, false);

    if (this->param_01_value != this->param_01_lastValue) {
        this->getEngine()->presetTouched();
        this->param_01_lastValue = this->param_01_value;
    }

    {
        list converted = {v};
        this->line_01_segments_set(converted);
    }
}

void toggle_01_valin_set(number v) {
    this->toggle_01_value_number_set(v);
}

void inport_01_value_bang_bang() {
    this->inport_01_out_bang_bang();
}

void inport_01_value_number_set(number v) {
    this->inport_01_out_number_set(v);
}

void inport_01_value_list_set(const list& v) {
    this->inport_01_out_list_set(v);
}

void numberobj_01_valin_set(number v) {
    this->numberobj_01_value_set(v);
}

void numberobj_01_format_set(number v) {
    this->numberobj_01_currentFormat = rnbo_trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void toggle_02_valin_set(number v) {
    this->toggle_02_value_number_set(v);
}

void metro_01_tick_bang() {
    this->metro_01_tickout_bang();
    this->getEngine()->flushClockEvents(this, 1935387534, false);;

    if ((bool)(this->metro_01_on)) {
        {
            this->getEngine()->scheduleClockEvent(this, 1935387534, this->metro_01_interval + this->_currentTime);;
        }
    }
}

void numberobj_02_valin_set(number v) {
    this->numberobj_02_value_set(v);
}

void numberobj_02_format_set(number v) {
    this->numberobj_02_currentFormat = rnbo_trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void delay_01_out_bang() {
    this->message_01_input_bang();
}

void message_01_listin_list_set(const list& v) {
    {
        list __value = jsCreateListCopy(v);
        this->message_01_input = jsCreateListCopy(__value);
    };
}

void message_01_listin_number_set(number v) {
    {
        list __value = v;
        this->message_01_input = jsCreateListCopy(__value);
    };
}

void message_01_listin_bang_bang() {
    this->message_01_input_bang();
}

void message_02_listin_list_set(const list& v) {
    {
        list __value = jsCreateListCopy(v);
        this->message_02_input = jsCreateListCopy(__value);
    };
}

void message_02_listin_number_set(number v) {
    {
        list __value = v;
        this->message_02_input = jsCreateListCopy(__value);
    };
}

void message_02_listin_bang_bang() {
    this->message_02_input_bang();
}

void message_03_listin_list_set(const list& v) {
    {
        list __value = jsCreateListCopy(v);
        this->message_03_input = jsCreateListCopy(__value);
    };
}

void message_03_listin_number_set(number v) {
    {
        list __value = v;
        this->message_03_input = jsCreateListCopy(__value);
    };
}

void message_03_listin_bang_bang() {
    this->message_03_input_bang();
}

void line_01_target_bang() {}

void line_02_target_bang() {}

number msToSamps(MillisecondTime ms, number sampleRate) {
    return ms * sampleRate * 0.001;
}

MillisecondTime sampsToMs(SampleIndex samps) {
    return samps * (this->invsr * 1000);
}

Index getMaxBlockSize() const {
    return this->maxvs;
}

number getSampleRate() const {
    return this->sr;
}

bool hasFixedVectorSize() const {
    return false;
}

Index getNumInputChannels() const {
    return 2;
}

Index getNumOutputChannels() const {
    return 2;
}

void allocateDataRefs() {
    this->cycle_tilde_01_buffer->requestSize(16384, 1);
    this->cycle_tilde_01_buffer->setSampleRate(this->sr);
    this->cycle_tilde_02_buffer->requestSize(16384, 1);
    this->cycle_tilde_02_buffer->setSampleRate(this->sr);
    this->cycle_tilde_01_buffer = this->cycle_tilde_01_buffer->allocateIfNeeded();
    this->cycle_tilde_02_buffer = this->cycle_tilde_02_buffer->allocateIfNeeded();

    if (this->RNBODefaultSinus->hasRequestedSize()) {
        if (this->RNBODefaultSinus->wantsFill())
            this->fillRNBODefaultSinus(this->RNBODefaultSinus);

        this->getEngine()->sendDataRefUpdated(0);
    }
}

void initializeObjects() {
    this->numberobj_01_init();
    this->ip_01_init();
    this->numberobj_02_init();
    this->message_01_init();
    this->change_01_init();
    this->message_02_init();
    this->message_03_init();
}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime());

    if ((bool)(this->metro_01_on))
        this->metro_01_on_set(1);

    {
        this->scheduleParamInit(0, 0);
    }

    this->processParamInitEvents();
}

static number param_01_value_constrain(number v) {
    v = (v > 0 ? 0 : (v < -70 ? -70 : v));
    return v;
}

void line_01_segments_set(const list& v) {
    this->line_01_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        auto currentTime = this->currentsampletime();
        number lastRampValue = this->line_01_currentValue;
        number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

        for (Index i = 0; i < this->line_01_activeRamps->length; i += 3) {
            rampEnd = this->line_01_activeRamps[(Index)(i + 2)];

            if (rampEnd > currentTime) {
                this->line_01_activeRamps[(Index)(i + 2)] = currentTime;
                number diff = rampEnd - currentTime;
                number valueDiff = diff * this->line_01_activeRamps[(Index)(i + 1)];
                lastRampValue = this->line_01_activeRamps[(Index)i] - valueDiff;
                this->line_01_activeRamps[(Index)i] = lastRampValue;
                this->line_01_activeRamps->length = i + 3;
                rampEnd = currentTime;
            } else {
                lastRampValue = this->line_01_activeRamps[(Index)i];
            }
        }

        if (rampEnd < currentTime) {
            this->line_01_activeRamps->push(lastRampValue);
            this->line_01_activeRamps->push(0);
            this->line_01_activeRamps->push(currentTime);
        }

        number lastRampEnd = currentTime;

        for (Index i = 0; i < v->length; i += 2) {
            number destinationValue = v[(Index)i];
            number inc = 0;
            number rampTimeInSamples;

            if (v->length > i + 1) {
                rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
            } else {
                rampTimeInSamples = this->mstosamps(this->line_01_time);
            }

            if (rampTimeInSamples <= 0)
                rampTimeInSamples = 1;

            inc = (destinationValue - lastRampValue) / rampTimeInSamples;
            lastRampEnd += rampTimeInSamples;
            this->line_01_activeRamps->push(destinationValue);
            this->line_01_activeRamps->push(inc);
            this->line_01_activeRamps->push(lastRampEnd);
            lastRampValue = destinationValue;
        }
    }
}

void message_01_set_set(const list& v) {
    {
        list __value = jsCreateListCopy(v);
        this->message_01_input = jsCreateListCopy(__value);
    };

    this->getEngine()->sendListMessage(TAG("listout"), TAG("message_obj-53"), v, this->_currentTime);
}

void delay_01_stop_bang() {
    this->getEngine()->flushClockEvents(this, 922084457, false);;
}

void delay_01_input_bang() {
    if ((bool)(!(bool)(this->delay_01_delayall)))
        this->delay_01_stop_bang();

    this->getEngine()->scheduleClockEvent(this, 922084457, this->delay_01_time + this->_currentTime);;
}

void select_01_match1_bang() {
    this->delay_01_input_bang();
}

void change_01_zero_set(number ) {}

void change_01_nonzero_set(number ) {}

void phasor_01_phase_set(number v) {
    if (v >= 0 && (bool)(this->phasor_01_sigbuf)) {
        this->phasor_01_sigbuf[(Index)this->sampleOffsetIntoNextAudioBuffer] = v;
    }
}

void outport_01_input_number_set(number v) {
    this->getEngine()->sendNumMessage(TAG("time"), TAG(""), v, this->_currentTime);
}

void numberobj_01_output_set(number v) {
    this->outport_01_input_number_set(v);
}

void numberobj_01_value_set(number v) {
    this->numberobj_01_value_setter(v);
    v = this->numberobj_01_value;
    number localvalue = v;

    if (this->numberobj_01_currentFormat != 6) {
        localvalue = rnbo_trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-80"), localvalue, this->_currentTime);
    this->numberobj_01_output_set(localvalue);
}

void expr_01_out1_set(number v) {
    this->expr_01_out1 = v;
    this->numberobj_01_value_set(this->expr_01_out1);
}

void expr_01_in1_set(number in1) {
    this->expr_01_in1 = in1;

    this->expr_01_out1_set(
        (this->expr_01_in2 == 0 ? 0 : rnbo_fround((this->expr_01_in2 == 0. ? 0. : this->expr_01_in1 * 1 / this->expr_01_in2)) * this->expr_01_in2)
    );//#map:round_obj-81:1
}

void expr_02_out1_set(number v) {
    this->expr_02_out1 = v;
    this->expr_01_in1_set(this->expr_02_out1);
}

void expr_02_in1_set(number in1) {
    this->expr_02_in1 = in1;
    this->expr_02_out1_set(this->expr_02_in1 * this->expr_02_in2);//#map:*_obj-76:1
}

void expr_02_in2_set(number v) {
    this->expr_02_in2 = v;
}

void message_02_out_set(const list& v) {
    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->phasor_01_phase_set(converted);
    }

    {
        if (v->length > 1)
            this->expr_02_in2_set(v[1]);

        number converted = (v->length > 0 ? v[0] : 0);
        this->expr_02_in1_set(converted);
    }
}

void message_02_input_bang() {
    list v = this->message_02_input;
    this->message_02_out_set(v);
}

void select_02_match1_bang() {
    this->message_02_input_bang();
}

void select_02_nomatch_number_set(number ) {}

void select_02_input_number_set(number v) {
    if (v == this->select_02_test1)
        this->select_02_match1_bang();
    else
        this->select_02_nomatch_number_set(v);
}

void dspexpr_07_in2_set(number v) {
    this->dspexpr_07_in2 = v;
}

void metro_01_on_set(number v) {
    this->metro_01_on = v;
    this->getEngine()->flushClockEvents(this, 1935387534, false);;

    if ((bool)(v)) {
        {
            this->getEngine()->scheduleClockEvent(this, 1935387534, 0 + this->_currentTime);;
        }
    }
}

void toggle_02_output_set(number v) {
    this->metro_01_on_set(v);
}

void toggle_02_value_number_set(number v) {
    this->toggle_02_value_number_setter(v);
    v = this->toggle_02_value_number;
    this->getEngine()->sendNumMessage(TAG("valout"), TAG("toggle_obj-73"), v, this->_currentTime);
    this->toggle_02_output_set(v);
}

void change_01_out_set(number v) {
    this->change_01_out = v;
    this->select_02_input_number_set(v);
    this->dspexpr_07_in2_set(v);
    this->toggle_02_value_number_set(v);
}

void change_01_input_set(number v) {
    this->change_01_input = v;

    if (v != this->change_01_prev) {
        number prev = this->change_01_prev;
        this->change_01_prev = v;

        if (v == 0) {
            this->change_01_zero_set(1);
        } else if (this->change_01_out == 0) {
            this->change_01_nonzero_set(1);
        }

        {
            this->change_01_out_set(v);
        }
    } else {
        this->change_01_prev = v;
    }
}

void numberobj_02_output_set(number v) {
    this->change_01_input_set(v);
}

void numberobj_02_value_set(number v) {
    this->numberobj_02_value_setter(v);
    v = this->numberobj_02_value;
    number localvalue = v;

    if (this->numberobj_02_currentFormat != 6) {
        localvalue = rnbo_trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-59"), localvalue, this->_currentTime);
    this->numberobj_02_output_set(localvalue);
}

void message_03_out_set(const list& v) {
    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_02_value_set(converted);
    }
}

void message_03_input_bang() {
    list v = this->message_03_input;
    this->message_03_out_set(v);
}

void select_01_match2_bang() {
    this->message_03_input_bang();
}

void select_01_nomatch_number_set(number ) {}

void select_01_input_number_set(number v) {
    if (v == this->select_01_test1)
        this->select_01_match1_bang();
    else if (v == this->select_01_test2)
        this->select_01_match2_bang();
    else
        this->select_01_nomatch_number_set(v);
}

void line_02_segments_set(const list& v) {
    this->line_02_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        auto currentTime = this->currentsampletime();
        number lastRampValue = this->line_02_currentValue;
        number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

        for (Index i = 0; i < this->line_02_activeRamps->length; i += 3) {
            rampEnd = this->line_02_activeRamps[(Index)(i + 2)];

            if (rampEnd > currentTime) {
                this->line_02_activeRamps[(Index)(i + 2)] = currentTime;
                number diff = rampEnd - currentTime;
                number valueDiff = diff * this->line_02_activeRamps[(Index)(i + 1)];
                lastRampValue = this->line_02_activeRamps[(Index)i] - valueDiff;
                this->line_02_activeRamps[(Index)i] = lastRampValue;
                this->line_02_activeRamps->length = i + 3;
                rampEnd = currentTime;
            } else {
                lastRampValue = this->line_02_activeRamps[(Index)i];
            }
        }

        if (rampEnd < currentTime) {
            this->line_02_activeRamps->push(lastRampValue);
            this->line_02_activeRamps->push(0);
            this->line_02_activeRamps->push(currentTime);
        }

        number lastRampEnd = currentTime;

        for (Index i = 0; i < v->length; i += 2) {
            number destinationValue = v[(Index)i];
            number inc = 0;
            number rampTimeInSamples;

            if (v->length > i + 1) {
                rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
            } else {
                rampTimeInSamples = this->mstosamps(this->line_02_time);
            }

            if (rampTimeInSamples <= 0)
                rampTimeInSamples = 1;

            inc = (destinationValue - lastRampValue) / rampTimeInSamples;
            lastRampEnd += rampTimeInSamples;
            this->line_02_activeRamps->push(destinationValue);
            this->line_02_activeRamps->push(inc);
            this->line_02_activeRamps->push(lastRampEnd);
            lastRampValue = destinationValue;
        }
    }
}

void toggle_01_output_set(number v) {
    {
        list converted = {v};
        this->message_01_set_set(converted);
    }

    this->select_01_input_number_set(v);

    {
        list converted = {v};
        this->line_02_segments_set(converted);
    }
}

void toggle_01_value_number_set(number v) {
    this->toggle_01_value_number_setter(v);
    v = this->toggle_01_value_number;
    this->getEngine()->sendNumMessage(TAG("valout"), TAG("toggle_obj-36"), v, this->_currentTime);
    this->toggle_01_output_set(v);
}

void toggle_01_value_bang_bang() {
    number val = (this->toggle_01_value_number == 1 ? 0 : 1);
    this->toggle_01_value_number_set(val);
}

void inport_01_out_bang_bang() {
    this->toggle_01_value_bang_bang();
}

void inport_01_out_number_set(number v) {
    this->toggle_01_value_number_set(v);
}

void inport_01_out_list_set(const list& v) {
    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->toggle_01_value_number_set(converted);
    }
}

void snapshot_01_out_set(number v) {
    this->snapshot_01_out = v;
    this->expr_02_in1_set(v);
}

void snapshot_01_input_bang_bang() {
    this->snapshot_01_out_set(this->snapshot_01_lastValue);
}

void metro_01_tickout_bang() {
    this->snapshot_01_input_bang_bang();
}

void message_01_out_set(const list& v) {
    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_02_value_set(converted);
    }
}

void message_01_input_bang() {
    list v = this->message_01_input;
    this->message_01_out_set(v);
}

void cycle_tilde_01_perform(
    number frequency,
    number phase_offset,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    RNBO_UNUSED(phase_offset);
    RNBO_UNUSED(frequency);
    auto __cycle_tilde_01_f2i = this->cycle_tilde_01_f2i;
    auto __cycle_tilde_01_phasei = this->cycle_tilde_01_phasei;
    Index i;

    for (i = 0; i < n; i++) {
        {
            uint32_t uint_phase;

            {
                {
                    uint_phase = __cycle_tilde_01_phasei;
                }
            }

            uint32_t idx = (uint32_t)(uint32_rshift(uint_phase, 18));
            number frac = ((BinOpInt)((UBinOpInt)uint_phase & (UBinOpInt)262143)) * 3.81471181759574e-6;
            number y0 = this->cycle_tilde_01_buffer[(Index)idx];
            number y1 = this->cycle_tilde_01_buffer[(Index)((UBinOpInt)(idx + 1) & (UBinOpInt)16383)];
            number y = y0 + frac * (y1 - y0);

            {
                uint32_t pincr = (uint32_t)(uint32_trunc(700 * __cycle_tilde_01_f2i));
                __cycle_tilde_01_phasei = uint32_add(__cycle_tilde_01_phasei, pincr);
            }

            out1[(Index)i] = y;
            out2[(Index)i] = uint_phase * 0.232830643653869629e-9;
            continue;
        }
    }

    this->cycle_tilde_01_phasei = __cycle_tilde_01_phasei;
}

void cycle_tilde_02_perform(
    number frequency,
    number phase_offset,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    RNBO_UNUSED(phase_offset);
    RNBO_UNUSED(frequency);
    auto __cycle_tilde_02_f2i = this->cycle_tilde_02_f2i;
    auto __cycle_tilde_02_phasei = this->cycle_tilde_02_phasei;
    Index i;

    for (i = 0; i < n; i++) {
        {
            uint32_t uint_phase;

            {
                {
                    uint_phase = __cycle_tilde_02_phasei;
                }
            }

            uint32_t idx = (uint32_t)(uint32_rshift(uint_phase, 18));
            number frac = ((BinOpInt)((UBinOpInt)uint_phase & (UBinOpInt)262143)) * 3.81471181759574e-6;
            number y0 = this->cycle_tilde_02_buffer[(Index)idx];
            number y1 = this->cycle_tilde_02_buffer[(Index)((UBinOpInt)(idx + 1) & (UBinOpInt)16383)];
            number y = y0 + frac * (y1 - y0);

            {
                uint32_t pincr = (uint32_t)(uint32_trunc(0.5 * __cycle_tilde_02_f2i));
                __cycle_tilde_02_phasei = uint32_add(__cycle_tilde_02_phasei, pincr);
            }

            out1[(Index)i] = y;
            out2[(Index)i] = uint_phase * 0.232830643653869629e-9;
            continue;
        }
    }

    this->cycle_tilde_02_phasei = __cycle_tilde_02_phasei;
}

void scale_tilde_01_perform(
    const Sample * x,
    number lowin,
    number hiin,
    number lowout,
    number highout,
    number pow,
    SampleValue * out1,
    Index n
) {
    RNBO_UNUSED(pow);
    RNBO_UNUSED(highout);
    RNBO_UNUSED(lowout);
    RNBO_UNUSED(hiin);
    RNBO_UNUSED(lowin);
    auto inscale = this->safediv(1., 1 - -1);
    number outdiff = 0 - 1;
    Index i;

    for (i = 0; i < n; i++) {
        number value = (x[(Index)i] - -1) * inscale;
        value = value * outdiff + 1;
        out1[(Index)i] = value;
    }
}

void scale_tilde_02_perform(
    const Sample * x,
    number lowin,
    number hiin,
    number lowout,
    number highout,
    number pow,
    SampleValue * out1,
    Index n
) {
    RNBO_UNUSED(pow);
    RNBO_UNUSED(highout);
    RNBO_UNUSED(lowout);
    RNBO_UNUSED(hiin);
    RNBO_UNUSED(lowin);
    auto inscale = this->safediv(1., 1 - -1);
    number outdiff = 1 - 0;
    Index i;

    for (i = 0; i < n; i++) {
        number value = (x[(Index)i] - -1) * inscale;
        value = value * outdiff + 0;
        out1[(Index)i] = value;
    }
}

void line_01_perform(SampleValue * out, Index n) {
    auto __line_01_currentValue = this->line_01_currentValue;
    Index i = 0;

    if ((bool)(this->line_01_activeRamps->length)) {
        while ((bool)(this->line_01_activeRamps->length) && i < n) {
            number destinationValue = this->line_01_activeRamps[0];
            number inc = this->line_01_activeRamps[1];
            number rampTimeInSamples = this->line_01_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __line_01_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->line_01_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->line_01_activeRamps->length))) this->getEngine()->scheduleClockEventWithValue(
                    this,
                    1937376702,
                    this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                    0
                );;
            }

            __line_01_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __line_01_currentValue;
        i++;
    }

    this->line_01_currentValue = __line_01_currentValue;
}

void dspexpr_06_perform(const Sample * in1, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = rnbo_pow(10, in1[(Index)i] * 0.05);//#map:_###_obj_###_:1
    }
}

void dspexpr_05_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void signaladder_01_perform(
    const SampleValue * in1,
    const SampleValue * in2,
    const SampleValue * in3,
    SampleValue * out,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out[(Index)i] = in1[(Index)i] + in2[(Index)i] + in3[(Index)i];
    }
}

void dspexpr_04_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void signaladder_02_perform(
    const SampleValue * in1,
    const SampleValue * in2,
    const SampleValue * in3,
    SampleValue * out,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out[(Index)i] = in1[(Index)i] + in2[(Index)i] + in3[(Index)i];
    }
}

void dspexpr_02_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void line_02_perform(SampleValue * out, Index n) {
    auto __line_02_currentValue = this->line_02_currentValue;
    Index i = 0;

    if ((bool)(this->line_02_activeRamps->length)) {
        while ((bool)(this->line_02_activeRamps->length) && i < n) {
            number destinationValue = this->line_02_activeRamps[0];
            number inc = this->line_02_activeRamps[1];
            number rampTimeInSamples = this->line_02_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __line_02_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->line_02_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->line_02_activeRamps->length))) this->getEngine()->scheduleClockEventWithValue(
                    this,
                    -4141894,
                    this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                    0
                );;
            }

            __line_02_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __line_02_currentValue;
        i++;
    }

    this->line_02_currentValue = __line_02_currentValue;
}

void dspexpr_03_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_01_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void ip_01_perform(SampleValue * out, Index n) {
    auto __ip_01_sigbuf = this->ip_01_sigbuf;
    auto __ip_01_lastValue = this->ip_01_lastValue;
    auto __ip_01_lastIndex = this->ip_01_lastIndex;

    for (Index i = 0; i < n; i++) {
        out[(Index)i] = ((SampleIndex)(i) >= __ip_01_lastIndex ? __ip_01_lastValue : __ip_01_sigbuf[(Index)i]);
    }

    __ip_01_lastIndex = 0;
    this->ip_01_lastIndex = __ip_01_lastIndex;
}

void dspexpr_07_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
    }
}

void phasor_01_perform(const Sample * freq, SampleValue * out, Index n) {
    auto __phasor_01_sigbuf = this->phasor_01_sigbuf;

    for (Index i = 0; i < n; i++) {
        out[(Index)i] = this->phasor_01_ph_next(freq[(Index)i], __phasor_01_sigbuf[(Index)i]);
        __phasor_01_sigbuf[(Index)i] = -1;
    }
}

void snapshot_01_perform(const SampleValue * input_signal, Index n) {
    auto __snapshot_01_lastValue = this->snapshot_01_lastValue;
    auto __snapshot_01_calc = this->snapshot_01_calc;
    auto __snapshot_01_count = this->snapshot_01_count;
    auto __snapshot_01_nextTime = this->snapshot_01_nextTime;
    auto __snapshot_01_interval = this->snapshot_01_interval;
    number timeInSamples = this->msToSamps(__snapshot_01_interval, this->sr);

    if (__snapshot_01_interval > 0) {
        for (Index i = 0; i < n; i++) {
            if (__snapshot_01_nextTime <= __snapshot_01_count + (SampleIndex)(i)) {
                {
                    __snapshot_01_calc = input_signal[(Index)i];
                }

                this->getEngine()->scheduleClockEventWithValue(
                    this,
                    -1508480176,
                    this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                    __snapshot_01_calc
                );;

                __snapshot_01_calc = 0;
                __snapshot_01_nextTime += timeInSamples;
            }
        }

        __snapshot_01_count += this->vs;
    }

    __snapshot_01_lastValue = input_signal[(Index)(n - 1)];
    this->snapshot_01_nextTime = __snapshot_01_nextTime;
    this->snapshot_01_count = __snapshot_01_count;
    this->snapshot_01_calc = __snapshot_01_calc;
    this->snapshot_01_lastValue = __snapshot_01_lastValue;
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

void toggle_01_value_number_setter(number v) {
    this->toggle_01_value_number = (v != 0 ? 1 : 0);
}

void numberobj_01_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_01_currentFormat != 6) {
        localvalue = rnbo_trunc(localvalue);
    }

    this->numberobj_01_value = localvalue;
}

void toggle_02_value_number_setter(number v) {
    this->toggle_02_value_number = (v != 0 ? 1 : 0);
}

void metro_01_interval_setter(number v) {
    this->metro_01_interval = (v > 0 ? v : 0);
}

void numberobj_02_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_02_currentFormat != 6) {
        localvalue = rnbo_trunc(localvalue);
    }

    this->numberobj_02_value = localvalue;
}

number cycle_tilde_01_ph_next(number freq, number reset) {
    {
        {
            if (reset >= 0.)
                this->cycle_tilde_01_ph_currentPhase = reset;
        }
    }

    number pincr = freq * this->cycle_tilde_01_ph_conv;

    if (this->cycle_tilde_01_ph_currentPhase < 0.)
        this->cycle_tilde_01_ph_currentPhase = 1. + this->cycle_tilde_01_ph_currentPhase;

    if (this->cycle_tilde_01_ph_currentPhase > 1.)
        this->cycle_tilde_01_ph_currentPhase = this->cycle_tilde_01_ph_currentPhase - 1.;

    number tmp = this->cycle_tilde_01_ph_currentPhase;
    this->cycle_tilde_01_ph_currentPhase += pincr;
    return tmp;
}

void cycle_tilde_01_ph_reset() {
    this->cycle_tilde_01_ph_currentPhase = 0;
}

void cycle_tilde_01_ph_dspsetup() {
    this->cycle_tilde_01_ph_conv = (number)1 / this->sr;
}

void cycle_tilde_01_dspsetup(bool force) {
    if ((bool)(this->cycle_tilde_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->cycle_tilde_01_phasei = 0;
    this->cycle_tilde_01_f2i = (number)4294967296 / this->samplerate();
    this->cycle_tilde_01_wrap = (long)(this->cycle_tilde_01_buffer->getSize()) - 1;
    this->cycle_tilde_01_setupDone = true;
    this->cycle_tilde_01_ph_dspsetup();
}

void cycle_tilde_01_bufferUpdated() {
    this->cycle_tilde_01_wrap = (long)(this->cycle_tilde_01_buffer->getSize()) - 1;
}

number cycle_tilde_02_ph_next(number freq, number reset) {
    {
        {
            if (reset >= 0.)
                this->cycle_tilde_02_ph_currentPhase = reset;
        }
    }

    number pincr = freq * this->cycle_tilde_02_ph_conv;

    if (this->cycle_tilde_02_ph_currentPhase < 0.)
        this->cycle_tilde_02_ph_currentPhase = 1. + this->cycle_tilde_02_ph_currentPhase;

    if (this->cycle_tilde_02_ph_currentPhase > 1.)
        this->cycle_tilde_02_ph_currentPhase = this->cycle_tilde_02_ph_currentPhase - 1.;

    number tmp = this->cycle_tilde_02_ph_currentPhase;
    this->cycle_tilde_02_ph_currentPhase += pincr;
    return tmp;
}

void cycle_tilde_02_ph_reset() {
    this->cycle_tilde_02_ph_currentPhase = 0;
}

void cycle_tilde_02_ph_dspsetup() {
    this->cycle_tilde_02_ph_conv = (number)1 / this->sr;
}

void cycle_tilde_02_dspsetup(bool force) {
    if ((bool)(this->cycle_tilde_02_setupDone) && (bool)(!(bool)(force)))
        return;

    this->cycle_tilde_02_phasei = 0;
    this->cycle_tilde_02_f2i = (number)4294967296 / this->samplerate();
    this->cycle_tilde_02_wrap = (long)(this->cycle_tilde_02_buffer->getSize()) - 1;
    this->cycle_tilde_02_setupDone = true;
    this->cycle_tilde_02_ph_dspsetup();
}

void cycle_tilde_02_bufferUpdated() {
    this->cycle_tilde_02_wrap = (long)(this->cycle_tilde_02_buffer->getSize()) - 1;
}

void param_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_01_value;
}

void param_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_01_value_set(preset["value"]);
}

void toggle_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->toggle_01_value_number;
}

void toggle_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->toggle_01_value_number_set(preset["value"]);
}

void numberobj_01_init() {
    this->numberobj_01_currentFormat = 0;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-80"), 1, this->_currentTime);
}

void numberobj_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_01_value;
}

void numberobj_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_01_value_set(preset["value"]);
}

void toggle_02_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->toggle_02_value_number;
}

void toggle_02_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->toggle_02_value_number_set(preset["value"]);
}

void metro_01_onTransportChanged(number ) {}

void metro_01_onBeatTimeChanged(number ) {}

void ip_01_init() {
    this->ip_01_lastValue = this->ip_01_value;
}

void ip_01_dspsetup(bool force) {
    if ((bool)(this->ip_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->ip_01_lastIndex = 0;
    this->ip_01_setupDone = true;
}

number phasor_01_ph_next(number freq, number reset) {
    {
        {
            if (reset >= 0.)
                this->phasor_01_ph_currentPhase = reset;
        }
    }

    number pincr = freq * this->phasor_01_ph_conv;

    if (this->phasor_01_ph_currentPhase < 0.)
        this->phasor_01_ph_currentPhase = 1. + this->phasor_01_ph_currentPhase;

    if (this->phasor_01_ph_currentPhase > 1.)
        this->phasor_01_ph_currentPhase = this->phasor_01_ph_currentPhase - 1.;

    number tmp = this->phasor_01_ph_currentPhase;
    this->phasor_01_ph_currentPhase += pincr;
    return tmp;
}

void phasor_01_ph_reset() {
    this->phasor_01_ph_currentPhase = 0;
}

void phasor_01_ph_dspsetup() {
    this->phasor_01_ph_conv = (number)1 / this->sr;
}

void phasor_01_dspsetup(bool force) {
    if ((bool)(this->phasor_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->phasor_01_conv = (number)1 / this->samplerate();
    this->phasor_01_setupDone = true;
    this->phasor_01_ph_dspsetup();
}

void numberobj_02_init() {
    this->numberobj_02_currentFormat = 0;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-59"), 1, this->_currentTime);
}

void numberobj_02_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_02_value;
}

void numberobj_02_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_02_value_set(preset["value"]);
}

void message_01_init() {
    {
        list __value = {0};
        this->message_01_input = jsCreateListCopy(__value);
    };
}

void change_01_init() {
    this->change_01_prev = this->change_01_input;
}

void message_02_init() {
    {
        list __value = {0};
        this->message_02_input = jsCreateListCopy(__value);
    };
}

void message_03_init() {
    {
        list __value = {1};
        this->message_03_input = jsCreateListCopy(__value);
    };
}

Index globaltransport_getSampleOffset(MillisecondTime time) {
    return this->mstosamps(this->maximum(0, time - this->getEngine()->getCurrentTime()));
}

number globaltransport_getTempoAtSample(SampleIndex sampleOffset) {
    return (sampleOffset >= 0 && sampleOffset < this->vs ? this->globaltransport_tempo[(Index)sampleOffset] : this->globaltransport_lastTempo);
}

number globaltransport_getStateAtSample(SampleIndex sampleOffset) {
    return (sampleOffset >= 0 && sampleOffset < this->vs ? this->globaltransport_state[(Index)sampleOffset] : this->globaltransport_lastState);
}

number globaltransport_getState(MillisecondTime time) {
    return this->globaltransport_getStateAtSample(this->globaltransport_getSampleOffset(time));
}

number globaltransport_getBeatTime(MillisecondTime time) {
    number i = 2;

    while (i < this->globaltransport_beatTimeChanges->length && this->globaltransport_beatTimeChanges[(Index)(i + 1)] <= time) {
        i += 2;
    }

    i -= 2;
    number beatTimeBase = this->globaltransport_beatTimeChanges[(Index)i];

    if (this->globaltransport_getState(time) == 0)
        return beatTimeBase;

    number beatTimeBaseMsTime = this->globaltransport_beatTimeChanges[(Index)(i + 1)];
    number diff = time - beatTimeBaseMsTime;
    return beatTimeBase + this->mstobeats(diff);
}

bool globaltransport_setTempo(MillisecondTime time, number tempo, bool notify) {
    if ((bool)(notify)) {
        this->processTempoEvent(time, tempo);
        this->globaltransport_notify = true;
    } else {
        Index offset = (Index)(this->globaltransport_getSampleOffset(time));

        if (this->globaltransport_getTempoAtSample(offset) != tempo) {
            this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime(time));
            this->globaltransport_beatTimeChanges->push(time);
            fillSignal(this->globaltransport_tempo, this->vs, tempo, offset);
            this->globaltransport_lastTempo = tempo;
            this->globaltransport_tempoNeedsReset = true;
            return true;
        }
    }

    return false;
}

number globaltransport_getTempo(MillisecondTime time) {
    return this->globaltransport_getTempoAtSample(this->globaltransport_getSampleOffset(time));
}

bool globaltransport_setState(MillisecondTime time, number state, bool notify) {
    if ((bool)(notify)) {
        this->processTransportEvent(time, TransportState(state));
        this->globaltransport_notify = true;
    } else {
        Index offset = (Index)(this->globaltransport_getSampleOffset(time));

        if (this->globaltransport_getStateAtSample(offset) != state) {
            fillSignal(this->globaltransport_state, this->vs, state, offset);
            this->globaltransport_lastState = TransportState(state);
            this->globaltransport_stateNeedsReset = true;

            if (state == 0) {
                this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime(time));
                this->globaltransport_beatTimeChanges->push(time);
            }

            return true;
        }
    }

    return false;
}

bool globaltransport_setBeatTime(MillisecondTime time, number beattime, bool notify) {
    if ((bool)(notify)) {
        this->processBeatTimeEvent(time, beattime);
        this->globaltransport_notify = true;
        return false;
    } else {
        bool beatTimeHasChanged = false;
        float oldBeatTime = (float)(this->globaltransport_getBeatTime(time));
        float newBeatTime = (float)(beattime);

        if (oldBeatTime != newBeatTime) {
            beatTimeHasChanged = true;
        }

        this->globaltransport_beatTimeChanges->push(beattime);
        this->globaltransport_beatTimeChanges->push(time);
        return beatTimeHasChanged;
    }
}

number globaltransport_getBeatTimeAtSample(SampleIndex sampleOffset) {
    auto msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getBeatTime(this->getEngine()->getCurrentTime() + msOffset);
}

array<number, 2> globaltransport_getTimeSignature(MillisecondTime time) {
    number i = 3;

    while (i < this->globaltransport_timeSignatureChanges->length && this->globaltransport_timeSignatureChanges[(Index)(i + 2)] <= time) {
        i += 3;
    }

    i -= 3;

    return {
        this->globaltransport_timeSignatureChanges[(Index)i],
        this->globaltransport_timeSignatureChanges[(Index)(i + 1)]
    };
}

array<number, 2> globaltransport_getTimeSignatureAtSample(SampleIndex sampleOffset) {
    auto msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getTimeSignature(this->getEngine()->getCurrentTime() + msOffset);
}

bool globaltransport_setTimeSignature(MillisecondTime time, number numerator, number denominator, bool notify) {
    if ((bool)(notify)) {
        this->processTimeSignatureEvent(time, (int)(numerator), (int)(denominator));
        this->globaltransport_notify = true;
    } else {
        array<number, 2> currentSig = this->globaltransport_getTimeSignature(time);

        if (currentSig[0] != numerator || currentSig[1] != denominator) {
            this->globaltransport_timeSignatureChanges->push(numerator);
            this->globaltransport_timeSignatureChanges->push(denominator);
            this->globaltransport_timeSignatureChanges->push(time);
            return true;
        }
    }

    return false;
}

void globaltransport_advance() {
    if ((bool)(this->globaltransport_tempoNeedsReset)) {
        fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
        this->globaltransport_tempoNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTempoEvent(this->globaltransport_lastTempo);
        }
    }

    if ((bool)(this->globaltransport_stateNeedsReset)) {
        fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
        this->globaltransport_stateNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTransportEvent(TransportState(this->globaltransport_lastState));
        }
    }

    if (this->globaltransport_beatTimeChanges->length > 2) {
        this->globaltransport_beatTimeChanges[0] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 2)];
        this->globaltransport_beatTimeChanges[1] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 1)];
        this->globaltransport_beatTimeChanges->length = 2;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendBeatTimeEvent(this->globaltransport_beatTimeChanges[0]);
        }
    }

    if (this->globaltransport_timeSignatureChanges->length > 3) {
        this->globaltransport_timeSignatureChanges[0] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 3)];
        this->globaltransport_timeSignatureChanges[1] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 2)];
        this->globaltransport_timeSignatureChanges[2] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 1)];
        this->globaltransport_timeSignatureChanges->length = 3;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTimeSignatureEvent(
                (int)(this->globaltransport_timeSignatureChanges[0]),
                (int)(this->globaltransport_timeSignatureChanges[1])
            );
        }
    }

    this->globaltransport_notify = false;
}

void globaltransport_dspsetup(bool force) {
    if ((bool)(this->globaltransport_setupDone) && (bool)(!(bool)(force)))
        return;

    fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
    this->globaltransport_tempoNeedsReset = false;
    fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
    this->globaltransport_stateNeedsReset = false;
    this->globaltransport_setupDone = true;
}

bool stackprotect_check() {
    this->stackprotect_count++;

    if (this->stackprotect_count > 128) {
        console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
        return true;
    }

    return false;
}

void updateTime(MillisecondTime time) {
    this->_currentTime = time;
    this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));

    if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;

    if (this->sampleOffsetIntoNextAudioBuffer < 0)
        this->sampleOffsetIntoNextAudioBuffer = 0;
}

void assign_defaults()
{
    scale_tilde_01_x = 0;
    scale_tilde_01_lowin = -1;
    scale_tilde_01_hiin = 1;
    scale_tilde_01_lowout = 1;
    scale_tilde_01_highout = 0;
    scale_tilde_01_pow = 1;
    dspexpr_01_in1 = 0;
    dspexpr_01_in2 = 0;
    dspexpr_02_in1 = 0;
    dspexpr_02_in2 = 0;
    dspexpr_03_in1 = 0;
    dspexpr_03_in2 = 0;
    dspexpr_04_in1 = 0;
    dspexpr_04_in2 = 0;
    dspexpr_05_in1 = 0;
    dspexpr_05_in2 = 0;
    cycle_tilde_01_frequency = 700;
    cycle_tilde_01_phase_offset = 0;
    scale_tilde_02_x = 0;
    scale_tilde_02_lowin = -1;
    scale_tilde_02_hiin = 1;
    scale_tilde_02_lowout = 0;
    scale_tilde_02_highout = 1;
    scale_tilde_02_pow = 1;
    cycle_tilde_02_frequency = 0.5;
    cycle_tilde_02_phase_offset = 0;
    dspexpr_06_in1 = 0;
    line_01_time = 50;
    param_01_value = -70;
    toggle_01_value_number = 0;
    toggle_01_value_number_setter(toggle_01_value_number);
    line_02_time = 100;
    expr_01_in1 = 0;
    expr_01_in2 = 1;
    expr_01_out1 = 0;
    numberobj_01_value = 0;
    numberobj_01_value_setter(numberobj_01_value);
    expr_02_in1 = 0;
    expr_02_in2 = 60;
    expr_02_out1 = 0;
    toggle_02_value_number = 0;
    toggle_02_value_number_setter(toggle_02_value_number);
    metro_01_on = 0;
    metro_01_interval = 1000;
    metro_01_interval_setter(metro_01_interval);
    snapshot_01_interval = 0;
    snapshot_01_out = 0;
    dspexpr_07_in1 = 0;
    dspexpr_07_in2 = 0;
    ip_01_value = 0.016667;
    ip_01_impulse = 0;
    phasor_01_freq = 0;
    numberobj_02_value = 0;
    numberobj_02_value_setter(numberobj_02_value);
    delay_01_time = 100;
    delay_01_delayall = 1;
    select_01_test1 = 0;
    select_01_test2 = 1;
    change_01_input = 0;
    change_01_out = 0;
    select_02_test1 = 0;
    _currentTime = 0;
    audioProcessSampleCount = 0;
    sampleOffsetIntoNextAudioBuffer = 0;
    zeroBuffer = nullptr;
    dummyBuffer = nullptr;
    signals[0] = nullptr;
    signals[1] = nullptr;
    signals[2] = nullptr;
    signals[3] = nullptr;
    signals[4] = nullptr;
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
    cycle_tilde_01_wrap = 0;
    cycle_tilde_01_ph_currentPhase = 0;
    cycle_tilde_01_ph_conv = 0;
    cycle_tilde_01_setupDone = false;
    cycle_tilde_02_wrap = 0;
    cycle_tilde_02_ph_currentPhase = 0;
    cycle_tilde_02_ph_conv = 0;
    cycle_tilde_02_setupDone = false;
    line_01_currentValue = -70;
    param_01_lastValue = 0;
    toggle_01_lastValue = 0;
    line_02_currentValue = 0;
    numberobj_01_currentFormat = 6;
    numberobj_01_lastValue = 0;
    toggle_02_lastValue = 0;
    snapshot_01_calc = 0;
    snapshot_01_nextTime = 0;
    snapshot_01_count = 0;
    snapshot_01_lastValue = 0;
    ip_01_lastIndex = 0;
    ip_01_lastValue = 0;
    ip_01_resetCount = 0;
    ip_01_sigbuf = nullptr;
    ip_01_setupDone = false;
    phasor_01_sigbuf = nullptr;
    phasor_01_lastLockedPhase = 0;
    phasor_01_conv = 0;
    phasor_01_ph_currentPhase = 0;
    phasor_01_ph_conv = 0;
    phasor_01_setupDone = false;
    numberobj_02_currentFormat = 6;
    numberobj_02_lastValue = 0;
    globaltransport_tempo = nullptr;
    globaltransport_tempoNeedsReset = false;
    globaltransport_lastTempo = 120;
    globaltransport_state = nullptr;
    globaltransport_stateNeedsReset = false;
    globaltransport_lastState = 0;
    globaltransport_beatTimeChanges = { 0, 0 };
    globaltransport_timeSignatureChanges = { 4, 4, 0 };
    globaltransport_notify = false;
    globaltransport_setupDone = false;
    stackprotect_count = 0;
    _voiceIndex = 0;
    _noteNumber = 0;
    isMuted = 1;
}

// member variables

    number scale_tilde_01_x;
    number scale_tilde_01_lowin;
    number scale_tilde_01_hiin;
    number scale_tilde_01_lowout;
    number scale_tilde_01_highout;
    number scale_tilde_01_pow;
    number dspexpr_01_in1;
    number dspexpr_01_in2;
    number dspexpr_02_in1;
    number dspexpr_02_in2;
    number dspexpr_03_in1;
    number dspexpr_03_in2;
    number dspexpr_04_in1;
    number dspexpr_04_in2;
    number dspexpr_05_in1;
    number dspexpr_05_in2;
    number cycle_tilde_01_frequency;
    number cycle_tilde_01_phase_offset;
    number scale_tilde_02_x;
    number scale_tilde_02_lowin;
    number scale_tilde_02_hiin;
    number scale_tilde_02_lowout;
    number scale_tilde_02_highout;
    number scale_tilde_02_pow;
    number cycle_tilde_02_frequency;
    number cycle_tilde_02_phase_offset;
    number dspexpr_06_in1;
    list line_01_segments;
    number line_01_time;
    number param_01_value;
    number toggle_01_value_number;
    list line_02_segments;
    number line_02_time;
    number expr_01_in1;
    number expr_01_in2;
    number expr_01_out1;
    number numberobj_01_value;
    number expr_02_in1;
    number expr_02_in2;
    number expr_02_out1;
    number toggle_02_value_number;
    number metro_01_on;
    number metro_01_interval;
    number snapshot_01_interval;
    number snapshot_01_out;
    number dspexpr_07_in1;
    number dspexpr_07_in2;
    number ip_01_value;
    number ip_01_impulse;
    number phasor_01_freq;
    number numberobj_02_value;
    number delay_01_time;
    number delay_01_delayall;
    list message_01_input;
    number select_01_test1;
    number select_01_test2;
    number change_01_input;
    number change_01_out;
    list message_02_input;
    number select_02_test1;
    list message_03_input;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    SampleValue * signals[5];
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    Float64BufferRef cycle_tilde_01_buffer;
    long cycle_tilde_01_wrap;
    uint32_t cycle_tilde_01_phasei;
    SampleValue cycle_tilde_01_f2i;
    number cycle_tilde_01_ph_currentPhase;
    number cycle_tilde_01_ph_conv;
    bool cycle_tilde_01_setupDone;
    Float64BufferRef cycle_tilde_02_buffer;
    long cycle_tilde_02_wrap;
    uint32_t cycle_tilde_02_phasei;
    SampleValue cycle_tilde_02_f2i;
    number cycle_tilde_02_ph_currentPhase;
    number cycle_tilde_02_ph_conv;
    bool cycle_tilde_02_setupDone;
    list line_01_activeRamps;
    number line_01_currentValue;
    number param_01_lastValue;
    number toggle_01_lastValue;
    list line_02_activeRamps;
    number line_02_currentValue;
    Int numberobj_01_currentFormat;
    number numberobj_01_lastValue;
    number toggle_02_lastValue;
    number snapshot_01_calc;
    number snapshot_01_nextTime;
    SampleIndex snapshot_01_count;
    number snapshot_01_lastValue;
    SampleIndex ip_01_lastIndex;
    number ip_01_lastValue;
    SampleIndex ip_01_resetCount;
    signal ip_01_sigbuf;
    bool ip_01_setupDone;
    signal phasor_01_sigbuf;
    number phasor_01_lastLockedPhase;
    number phasor_01_conv;
    number phasor_01_ph_currentPhase;
    number phasor_01_ph_conv;
    bool phasor_01_setupDone;
    Int numberobj_02_currentFormat;
    number numberobj_02_lastValue;
    number change_01_prev;
    signal globaltransport_tempo;
    bool globaltransport_tempoNeedsReset;
    number globaltransport_lastTempo;
    signal globaltransport_state;
    bool globaltransport_stateNeedsReset;
    number globaltransport_lastState;
    list globaltransport_beatTimeChanges;
    list globaltransport_timeSignatureChanges;
    bool globaltransport_notify;
    bool globaltransport_setupDone;
    number stackprotect_count;
    DataRef RNBODefaultSinus;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;

};

PatcherInterface* creaternbomatic()
{
    return new rnbomatic();
}

#ifndef RNBO_NO_PATCHERFACTORY

extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction(PlatformInterface* platformInterface)
#else

extern "C" PatcherFactoryFunctionPtr rnbomaticFactoryFunction(PlatformInterface* platformInterface)
#endif

{
    Platform::set(platformInterface);
    return creaternbomatic;
}

} // end RNBO namespace

