#ifndef WRC_MIC_BLOW_H
#define WRC_MIC_BLOW_H

#ifdef __cplusplus
extern "C" {
#endif

/* Raw 8-bit unsigned PCM samples captured from a real DS mic "blow"
 * recording, embedded so the core has no runtime file dependency.
 * Sample rate is wrc_mic_blow_sample_rate (not 44100); resample when
 * consuming it.
 */
extern const unsigned char wrc_mic_blow_data[];
extern const unsigned int wrc_mic_blow_data_len;
extern const unsigned int wrc_mic_blow_sample_rate;

#ifdef __cplusplus
}
#endif

#endif
