/*
 *  models.cpp
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "models.h"

// Generated includes
#include "config.h"

// Stimulator models
#include "noise_generator.h"
#include "spike_generator.h"
#include "poisson_generator_ps.h"
#include "ac_generator.h"
#include "ppd_sup_generator.h"
#include "poisson_generator.h"
#include "dc_generator.h"
#include "pulsepacket_generator.h"
#include "step_current_generator.h"
#include "mip_generator.h"
#include "inhomogeneous_poisson_generator.h"
#include "step_rate_generator.h"
#include "sinusoidal_poisson_generator.h"
#include "gamma_sup_generator.h"
#if defined( HAVE_GSL )
#include "sinusoidal_gamma_generator.h"
#endif

// Neuron models
#if defined( HAVE_GSL )
#include "gif_cond_exp.h"
#include "glif_cond.h"
#include "iaf_bw_2001_exact.h"
#include "hh_cond_beta_gap_traub.h"
#include "iaf_cond_beta.h"
#include "gif_cond_exp_multisynapse.h"
#include "aeif_cond_alpha.h"
#include "hh_psc_alpha.h"
#include "aeif_cond_exp.h"
#include "iaf_cond_alpha.h"
#include "siegert_neuron.h"
#include "iaf_cond_exp.h"
#include "aeif_psc_exp.h"
#include "hh_cond_exp_traub.h"
#include "aeif_psc_delta.h"
#include "aeif_psc_alpha.h"
#include "iaf_cond_exp_sfa_rr.h"
#include "aeif_cond_alpha_astro.h"
#include "iaf_cond_alpha_mc.h"
#include "hh_psc_alpha_gap.h"
#include "iaf_chxk_2008.h"
#include "aeif_cond_alpha_multisynapse.h"
#include "astrocyte_lr_1994.h"
#include "ht_neuron.h"
#include "aeif_cond_beta_multisynapse.h"
#endif
#include "iaf_psc_alpha.h"
#include "iaf_psc_exp_htum.h"
#include "eprop_iaf_bsshslm_2020.h"
#include "iaf_psc_exp_ps_lossless.h"
#include "glif_psc.h"
#include "amat2_psc_exp.h"
#include "iaf_chs_2007.h"
#include "iaf_psc_alpha_multisynapse.h"
#include "iaf_psc_delta_ps.h"
#include "izhikevich.h"
#include "ignore_and_fire.h"
#include "iaf_psc_alpha_ps.h"
#include "pp_psc_delta.h"
#include "iaf_psc_delta.h"
#include "iaf_tum_2000.h"
#include "parrot_neuron_ps.h"
#include "iaf_psc_exp_ps.h"
#include "eprop_readout_bsshslm_2020.h"
#include "mat2_psc_exp.h"
#include "cm_default.h"
#include "iaf_psc_exp.h"
#include "eprop_iaf_adapt_bsshslm_2020.h"
#include "gif_psc_exp_multisynapse.h"
#include "glif_psc_double_alpha.h"
#include "parrot_neuron.h"
#include "gif_psc_exp.h"
#include "iaf_psc_exp_multisynapse.h"
#if defined( HAVE_GSL ) && defined( HAVE_BOOST )
#include "iaf_bw_2001.h"
#endif

// Clopath models
#if defined( HAVE_GSL )
#include "aeif_psc_delta_clopath.h"
#include "hh_psc_alpha_clopath.h"
#endif

// Connection models
#include "quantal_stp_synapse.h"
#include "stdp_facetshw_synapse_hom.h"
#include "rate_connection_instantaneous.h"
#include "ht_synapse.h"
#include "tsodyks_synapse_hom.h"
#include "vogels_sprekeler_synapse.h"
#include "sic_connection.h"
#include "eprop_learning_signal_connection_bsshslm_2020.h"
#include "rate_connection_delayed.h"
#include "static_synapse_hom_w.h"
#include "stdp_synapse_hom.h"
#include "gap_junction.h"
#include "stdp_dopamine_synapse.h"
#include "cont_delay_synapse_impl.h"
#include "diffusion_connection.h"
#include "stdp_nn_restr_synapse.h"
#include "stdp_pl_synapse_hom.h"
#include "urbanczik_synapse.h"
#include "stdp_triplet_synapse.h"
#include "stdp_nn_pre_centered_synapse.h"
#include "clopath_synapse.h"
#include "tsodyks2_synapse.h"
#include "stdp_facetshw_synapse_hom_impl.h"
#include "stdp_synapse.h"
#include "static_synapse.h"
#include "eprop_synapse_bsshslm_2020.h"
#include "tsodyks_synapse.h"
#include "quantal_stp_synapse_impl.h"
#include "jonke_synapse.h"
#include "cont_delay_synapse.h"
#include "stdp_nn_symm_synapse.h"
#include "bernoulli_synapse.h"

// Node models
#include "correlomatrix_detector.h"
#include "correlation_detector.h"
#include "correlospinmatrix_detector.h"
#include "volume_transmitter.h"
#include "spike_train_injector.h"
#if defined( HAVE_GSL )
#include "gif_pop_psc_exp.h"
#endif

// Binary models
#include "ginzburg_neuron.h"
#include "erfc_neuron.h"
#include "mcculloch_pitts_neuron.h"

// Rate models
#include "threshold_lin_rate.h"
#include "sigmoid_rate.h"
#include "lin_rate.h"
#include "gauss_rate.h"
#include "tanh_rate.h"
#include "sigmoid_rate_gg_1998.h"

// Recorder models
#include "spike_recorder.h"
#include "multimeter.h"
#include "spin_detector.h"
#include "weight_recorder.h"

// Devicelike models
#if defined( HAVE_MUSIC )
#include "music_cont_out_proxy.h"
#include "music_rate_out_proxy.h"
#include "music_rate_in_proxy.h"
#include "music_message_in_proxy.h"
#include "music_event_out_proxy.h"
#include "music_cont_in_proxy.h"
#include "music_event_in_proxy.h"
#endif
#include "spike_dilutor.h"

// Urbanczik models
#if defined( HAVE_GSL )
#include "pp_cond_exp_mc_urbanczik.h"
#endif

void nest::register_models()
{
  // Stimulator models
  register_ac_generator( "ac_generator" );
  register_dc_generator( "dc_generator" );
  register_gamma_sup_generator( "gamma_sup_generator" );
  register_mip_generator( "mip_generator" );
  register_noise_generator( "noise_generator" );
  register_inhomogeneous_poisson_generator( "inhomogeneous_poisson_generator" );
  register_poisson_generator( "poisson_generator" );
  register_poisson_generator_ps( "poisson_generator_ps" );
  register_ppd_sup_generator( "ppd_sup_generator" );
  register_pulsepacket_generator( "pulsepacket_generator" );
  register_sinusoidal_poisson_generator( "sinusoidal_poisson_generator" );
  register_spike_generator( "spike_generator" );
  register_step_current_generator( "step_current_generator" );
  register_step_rate_generator( "step_rate_generator" );
#if defined( HAVE_GSL )
  register_sinusoidal_gamma_generator( "sinusoidal_gamma_generator" );
#endif

  // Neuron models
#if defined( HAVE_GSL )
  register_aeif_cond_alpha( "aeif_cond_alpha" );
  register_aeif_cond_alpha_astro( "aeif_cond_alpha_astro" );
  register_aeif_cond_alpha_multisynapse( "aeif_cond_alpha_multisynapse" );
  register_aeif_cond_beta_multisynapse( "aeif_cond_beta_multisynapse" );
  register_aeif_cond_exp( "aeif_cond_exp" );
  register_aeif_psc_alpha( "aeif_psc_alpha" );
  register_aeif_psc_exp( "aeif_psc_exp" );
  register_aeif_psc_delta( "aeif_psc_delta" );
  register_astrocyte_lr_1994( "astrocyte_lr_1994" );
  register_gif_cond_exp( "gif_cond_exp" );
  register_gif_cond_exp_multisynapse( "gif_cond_exp_multisynapse" );
  register_glif_cond( "glif_cond" );
  register_hh_cond_exp_traub( "hh_cond_exp_traub" );
  register_hh_cond_beta_gap_traub( "hh_cond_beta_gap_traub" );
  register_hh_psc_alpha( "hh_psc_alpha" );
  register_hh_psc_alpha_gap( "hh_psc_alpha_gap" );
  register_ht_neuron( "ht_neuron" );
  register_iaf_chxk_2008( "iaf_chxk_2008" );
  register_iaf_cond_alpha( "iaf_cond_alpha" );
  register_iaf_cond_alpha_mc( "iaf_cond_alpha_mc" );
  register_iaf_cond_beta( "iaf_cond_beta" );
  register_iaf_cond_exp( "iaf_cond_exp" );
  register_iaf_cond_exp_sfa_rr( "iaf_cond_exp_sfa_rr" );
  register_iaf_bw_2001_exact( "iaf_bw_2001_exact" );
  register_siegert_neuron( "siegert_neuron" );
#endif
  register_amat2_psc_exp( "amat2_psc_exp" );
  register_cm_default( "cm_default" );
  register_eprop_iaf_bsshslm_2020( "eprop_iaf_bsshslm_2020" );
  register_eprop_iaf_adapt_bsshslm_2020( "eprop_iaf_adapt_bsshslm_2020" );
  register_eprop_readout_bsshslm_2020( "eprop_readout_bsshslm_2020" );
  register_gif_psc_exp( "gif_psc_exp" );
  register_gif_psc_exp_multisynapse( "gif_psc_exp_multisynapse" );
  register_glif_psc( "glif_psc" );
  register_glif_psc_double_alpha( "glif_psc_double_alpha" );
  register_iaf_chs_2007( "iaf_chs_2007" );
  register_iaf_psc_alpha( "iaf_psc_alpha" );
  register_iaf_psc_alpha_multisynapse( "iaf_psc_alpha_multisynapse" );
  register_iaf_psc_alpha_ps( "iaf_psc_alpha_ps" );
  register_iaf_psc_delta( "iaf_psc_delta" );
  register_iaf_psc_delta_ps( "iaf_psc_delta_ps" );
  register_iaf_psc_exp( "iaf_psc_exp" );
  register_iaf_psc_exp_htum( "iaf_psc_exp_htum" );
  register_iaf_psc_exp_multisynapse( "iaf_psc_exp_multisynapse" );
  register_iaf_psc_exp_ps( "iaf_psc_exp_ps" );
  register_iaf_psc_exp_ps_lossless( "iaf_psc_exp_ps_lossless" );
  register_iaf_tum_2000( "iaf_tum_2000" );
  register_ignore_and_fire( "ignore_and_fire" );
  register_izhikevich( "izhikevich" );
  register_mat2_psc_exp( "mat2_psc_exp" );
  register_parrot_neuron( "parrot_neuron" );
  register_parrot_neuron_ps( "parrot_neuron_ps" );
  register_pp_psc_delta( "pp_psc_delta" );
#if defined( HAVE_GSL ) && defined( HAVE_BOOST )
  register_iaf_bw_2001( "iaf_bw_2001" );
#endif

  // Clopath models
#if defined( HAVE_GSL )
  register_aeif_psc_delta_clopath( "aeif_psc_delta_clopath" );
  register_hh_psc_alpha_clopath( "hh_psc_alpha_clopath" );
#endif

  // Connection models
  register_bernoulli_synapse( "bernoulli_synapse" );
  register_clopath_synapse( "clopath_synapse" );
  register_cont_delay_synapse( "cont_delay_synapse" );
  register_diffusion_connection( "diffusion_connection" );
  register_eprop_synapse_bsshslm_2020( "eprop_synapse_bsshslm_2020" );
  register_eprop_learning_signal_connection_bsshslm_2020( "eprop_learning_signal_connection_bsshslm_2020" );
  register_gap_junction( "gap_junction" );
  register_ht_synapse( "ht_synapse" );
  register_jonke_synapse( "jonke_synapse" );
  register_quantal_stp_synapse( "quantal_stp_synapse" );
  register_rate_connection_delayed( "rate_connection_delayed" );
  register_rate_connection_instantaneous( "rate_connection_instantaneous" );
  register_sic_connection( "sic_connection" );
  register_static_synapse( "static_synapse" );
  register_static_synapse_hom_w( "static_synapse_hom_w" );
  register_stdp_dopamine_synapse( "stdp_dopamine_synapse" );
  register_stdp_nn_pre_centered_synapse( "stdp_nn_pre_centered_synapse" );
  register_stdp_nn_restr_synapse( "stdp_nn_restr_synapse" );
  register_stdp_nn_symm_synapse( "stdp_nn_symm_synapse" );
  register_stdp_pl_synapse_hom( "stdp_pl_synapse_hom" );
  register_stdp_synapse( "stdp_synapse" );
  register_stdp_facetshw_synapse_hom( "stdp_facetshw_synapse_hom" );
  register_stdp_synapse_hom( "stdp_synapse_hom" );
  register_stdp_triplet_synapse( "stdp_triplet_synapse" );
  register_tsodyks2_synapse( "tsodyks2_synapse" );
  register_tsodyks_synapse( "tsodyks_synapse" );
  register_tsodyks_synapse_hom( "tsodyks_synapse_hom" );
  register_urbanczik_synapse( "urbanczik_synapse" );
  register_vogels_sprekeler_synapse( "vogels_sprekeler_synapse" );

  // Node models
  register_correlation_detector( "correlation_detector" );
  register_correlomatrix_detector( "correlomatrix_detector" );
  register_correlospinmatrix_detector( "correlospinmatrix_detector" );
  register_spike_train_injector( "spike_train_injector" );
  register_volume_transmitter( "volume_transmitter" );
#if defined( HAVE_GSL )
  register_gif_pop_psc_exp( "gif_pop_psc_exp" );
#endif

  // Binary models
  register_erfc_neuron( "erfc_neuron" );
  register_ginzburg_neuron( "ginzburg_neuron" );
  register_mcculloch_pitts_neuron( "mcculloch_pitts_neuron" );

  // Rate models
  register_gauss_rate_ipn( "gauss_rate_ipn" );
  register_rate_transformer_gauss( "rate_transformer_gauss" );
  register_lin_rate_ipn( "lin_rate_ipn" );
  register_lin_rate_opn( "lin_rate_opn" );
  register_rate_transformer_lin( "rate_transformer_lin" );
  register_sigmoid_rate_ipn( "sigmoid_rate_ipn" );
  register_rate_transformer_sigmoid( "rate_transformer_sigmoid" );
  register_sigmoid_rate_gg_1998_ipn( "sigmoid_rate_gg_1998_ipn" );
  register_rate_transformer_sigmoid_gg_1998( "rate_transformer_sigmoid_gg_1998" );
  register_tanh_rate_ipn( "tanh_rate_ipn" );
  register_tanh_rate_opn( "tanh_rate_opn" );
  register_rate_transformer_tanh( "rate_transformer_tanh" );
  register_threshold_lin_rate_ipn( "threshold_lin_rate_ipn" );
  register_threshold_lin_rate_opn( "threshold_lin_rate_opn" );
  register_rate_transformer_threshold_lin( "rate_transformer_threshold_lin" );

  // Recorder models
  register_multimeter( "multimeter" );
  register_voltmeter( "voltmeter" );
  register_spike_recorder( "spike_recorder" );
  register_spin_detector( "spin_detector" );
  register_weight_recorder( "weight_recorder" );

  // Devicelike models
#if defined( HAVE_MUSIC )
  register_music_cont_in_proxy( "music_cont_in_proxy" );
  register_music_cont_out_proxy( "music_cont_out_proxy" );
  register_music_event_in_proxy( "music_event_in_proxy" );
  register_music_event_out_proxy( "music_event_out_proxy" );
  register_music_rate_in_proxy( "music_rate_in_proxy" );
  register_music_rate_out_proxy( "music_rate_out_proxy" );
  register_music_message_in_proxy( "music_message_in_proxy" );
#endif
  register_spike_dilutor( "spike_dilutor" );

  // Urbanczik models
#if defined( HAVE_GSL )
  register_pp_cond_exp_mc_urbanczik( "pp_cond_exp_mc_urbanczik" );
#endif
}