/* SHT Driver Version: 0.9.6
 *
 * Copyright (c) 2016 Sensirion AG.
 * All rights reserved.
 *
 * CONFIDENTIAL SOURCE CODE
 *
 * The software contained in this file is only available under the Sensirion
 * Special Software License Agreement (SLA) that has been accepted by Licensee.
 * A copy of the SLA is delivered along with this code for your reference. The
 * SLA regulates among others the use, incorporation into products,
 * distribution, etc. (collectively "Use") of this software, and the limitation
 * or exclusion of Sensirion's warranty and liability.
 * The SLA defines this software that it governs as The Software, and the
 * licensee as Licensee. See terms of the SLA. Licensee is legally bound to the
 * terms and conditions mentioned in the SLA. If in disagreement or if
 * compliance to the SLA cannot be guaranteed, the Use of The Software is
 * forbidden, The Software must be returned immediately to Sensirion, and any
 * and all copies of The Software shall be deleted. By using The Software
 * Licensee confirms its acceptance of the SLA and its compliance with the terms
 * of the SLA.
 * In particular (SLA terms apply, see SLA for details):
 * The Software contains confidential, proprietary information of Sensirion.
 * Disclosure to third parties is prohibited.
 * The Software source code must not be distributed and must not be reverse
 * engineered.
 * The Software source code shall only be used and adapted as stated in
 * writing in the application notes Sensirion delivered with The Software.
 * The Software shall only be used in conjunction with the Sensirion hardware
 * components it has been created for.
 *
 * The Software herein has been created for use with Sensirion SHT3x
 * sensor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES AND
 * REPRESENTATIONS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE OF THE SOFTWARE, ARE
 * HEREBY DISCLAIMED. EXCEPT FOR WILLFUL ACTS AND TO THE GREATEST EXTENT
 * PERMITTED BY APPLICABLE LAW, IN NO EVENT SHALL SENSIRION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, CLAIMS OF INTELLECTUAL PROPERTY
 * INFRINGEMENT, OR LOSS OF USE, DATA, OR PROFITS, OR BUSINESS INTERRUPTION,
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THE SOFTWARE OR PARTS THEREOF, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * For full SLA details please see SLA in Folder "5 Legal Documents" of this
 * Composer Kit.
 */

#ifdef Bran_R8


#ifndef SHT_COMPENSATION_CONFIGURATION_H
#define SHT_COMPENSATION_CONFIGURATION_H


#include <stdio.h>
#include <stdlib.h>
#include "sht_compensation_configuration.h"
#include "globalDefine.h"
#include "sensirion_configuration.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Sensirion Temperature+Humidity Compensation
 * Version: cleargrass-iaq-project-name-bran_v5
 */

void get_status_charging_on(int32_t *status_charging_on);

void get_status_CPU_load_CPU_f(int32_t *status_CPU_load_CPU_f);

void get_status_LCD_bri(int32_t *status_LCD_bri);

void get_status_CPU_load(int *status_CPU_load);

void get_status_CPU_f(int *status_CPU_f);

#ifdef __cplusplus
}
#endif

#endif /* SHT_COMPENSATION_CONFIGURATION_H */

#endif

