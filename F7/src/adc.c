#include "F7/adc.h"

////////// Timer driver ///////////

static const GPTConfig gpt_cfg = {
  50000,          //50 kHz, the frequency rate of a music file in WAV format (we wanted 44.1kHz, but that doesn't seem to be possible)
  NULL,           // no callback
  TIM_CR2_MMS_1,  // MMS = 010 = TRGO on Update Event, to enable use of the timer as a trigger for ADC
  0
};

////////// ADC driver /////////////

#define ADC_NUM_CHANNELS 2
#define ADC_BUF_DEPTH 256

static int adc_voltage_range = 1800;
// ADC resolution is 12 bits, so step value is 2**(-12)
static int adc_resolution = 4096;

static adcsample_t buffer_adc[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];

static adcsample_t * buffer_to_read;
static int buf_processed;

static void adc_cb(ADCDriver* adcp, adcsample_t *buffer, size_t n){
  (void)adcp;
  (void)buffer;
  (void)n;

  //SEGGER_RTT_WriteString(0, "Half buffer reached!");

  if(buffer_to_read != buffer_adc) {
    //SEGGER_RTT_printf(0, "Full buffer reached!\r\n");
    buffer_to_read = buffer_adc;
    buf_processed = 0;
  } else {
    //SEGGER_RTT_printf(0, "Half buffer reached!\r\n");
    buffer_to_read = buffer_adc + ADC_BUF_DEPTH/2;
    buf_processed = 0;
  }


}

static void adc_err_cb(ADCDriver* adcp, adcerror_t err) {
  (void)err;
  (void)adcp;
}

static const ADCConversionGroup adc_grp_cfg = {
  // Using a circular buffer for continuous sampling
  TRUE,
  // Number of channels (we use two separate ADCs)
  ADC_NUM_CHANNELS,
  // Callback for half-filled buffer interrupt
  adc_cb,
  // Callback for errors of ADC (never used)
  adc_err_cb,
  //Control register 1 (ADC_CR1)
  0,
  //Control register 2 (ADC_CR2) : conversion triggered by TIM4 (F7)/TIM3 (F4) rising edge
  ADC_CR2_EXTEN_RISING | ADC_CR2_EXTSEL_SRC(ADC_TRIG_ORIG),
  //Sample time register 1 (ADC_SMPR1)
  0,
  //Sample time register 2 (ADC_SMPR2): 3 cycles for one sample on channels 8 and 9
  ADC_SAMPLE_RATE,
  //Conversion sequence register 1 (ADC_SQR1)
  0,
  //Conversion sequence register 2 (ADC_SQR2)
  0,
  //Conversion sequence register 3 (ADC_SQR3): we first convert the channel 8, then the channel 9
  ADC_CONV_SEQUENCE
};


void lsm_adc_init(void){
  palSetPadMode(ADC_RIGHT_PORT, ADC_RIGHT_PIN, PAL_MODE_INPUT_ANALOG);
  palSetPadMode(ADC_LEFT_PORT, ADC_LEFT_PIN, PAL_MODE_INPUT_ANALOG);
  adcStart(&ADC_DRIVER, NULL);
  gptStart(&ADC_GPT, &gpt_cfg);
}

static THD_WORKING_AREA(lsm_wa_adc_test_thread, 512);

static THD_FUNCTION(lsm_adc_test_thread, arg) {
  (void)arg;
  for(;;){
    buf_processed = 1;
    while(buf_processed)
      chThdSleepMicroseconds(1);
    SEGGER_RTT_printf(0, "Received sample from ADC! \r\n");
    SEGGER_RTT_printf(0, "Left (mV)\tRight (mV)\r\n");
    for(int i = 0; i < ADC_BUF_DEPTH/2-1; i += 2) {
      SEGGER_RTT_printf(0, "%d\t\t%d\r\n", (int) (buffer_to_read[i]*adc_voltage_range/adc_resolution), (int) (buffer_to_read[i+1]*adc_voltage_range/adc_resolution));
    }
  }
}

//The test continuously prints the sampled value in the input pins in millivolts on the RTT
void lsm_adc_test(void){

  lsm_adc_init();
  adcStartConversion(&ADC_DRIVER, &adc_grp_cfg, buffer_adc, ADC_BUF_DEPTH);
  gptStartContinuous(&ADC_GPT, 1);

  SEGGER_RTT_WriteString(0, "Conversion started\r\n");

  chThdCreateStatic(lsm_wa_adc_test_thread, sizeof(lsm_wa_adc_test_thread), NORMALPRIO + 5, lsm_adc_test_thread, NULL);
}
