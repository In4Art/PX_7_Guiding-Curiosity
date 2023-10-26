/*
 *
 * A simple library for some simple "waveform" generators. 
 * Not optimized, and for now only a simple triangle generator implemented.
 * 
 * 
 */




class TriGen {
    private:
        float _ramp;
        float _incr;
        float _max;
        float _sRate; //sample rate of system. Needed to properly calculate waveforms from frequency inputs
        void (*_periodCB)(TriGen *genPtr);
    public:
        TriGen();
        TriGen(float freq, float max, float samprate);

        void setFreq(float freq);
        void setMax(float max);
        void setSamprate(float samprate);
        void setPeriodCB(void (*callBack)(TriGen *genPtr));
        float calc(void);

};