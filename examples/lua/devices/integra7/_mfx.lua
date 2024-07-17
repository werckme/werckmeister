Mfx_Table = {
    {
        desc = 'THRU',
        leaf = {
            { desc= 'Control Assign 1', id= 'MFX_THRU_CTRL_ASGN1', init= 0, min= 0, max= 0 },
            { desc= 'Control Assign 2', id= 'MFX_THRU_CTRL_ASGN2', init= 0, min= 0, max= 0 },
            { desc= 'Control Assign 3', id= 'MFX_THRU_CTRL_ASGN3', init= 0, min= 0, max= 0 },
            { desc= 'Control Assign 4', id= 'MFX_THRU_CTRL_ASGN4', init= 0, min= 0, max= 0 },
            { desc= '_', id= 'MFX_THRU_DUMMY', init= 32768, min= 32768, max= 32768 }
        }
    },
    {
        desc= 'EQUALIZER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_STEQ_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_STEQ_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_STEQ_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_STEQ_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Low Freq', id= 'MFX_STEQ_LO_FREQ', init= 32769, min= 32768, max= 32769 },
            { desc= 'Low Gain', id= 'MFX_STEQ_LO_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'Mid1 Freq', id= 'MFX_STEQ_MID1_FREQ', init= 32775, min= 32768, max= 32784 },
            { desc= 'Mid1 Gain', id= 'MFX_STEQ_MID1_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'Mid1 Q', id= 'MFX_STEQ_MID1_Q', init= 32768, min= 32768, max= 32772 },
            { desc= 'Mid2 Freq', id= 'MFX_STEQ_MID2_FREQ', init= 32778, min= 32768, max= 32784 },
            { desc= 'Mid2 Gain', id= 'MFX_STEQ_MID2_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'Mid2 Q', id= 'MFX_STEQ_MID2_Q', init= 32768, min= 32768, max= 32772 },
            { desc= 'High Freq', id= 'MFX_STEQ_HI_FREQ', init= 32769, min= 32768, max= 32770 },
            { desc= 'High Gain', id= 'MFX_STEQ_HI_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_STEQ_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'SPECTRUM',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_SPECT_CTRL_ASGN1', init= 0, min= 0, max= 1 },
            { desc= 'Control Assign 2', id= 'MFX_SPECT_CTRL_ASGN2', init= 0, min= 0, max= 1 },
            { desc= 'Control Assign 3', id= 'MFX_SPECT_CTRL_ASGN3', init= 0, min= 0, max= 1 },
            { desc= 'Control Assign 4', id= 'MFX_SPECT_CTRL_ASGN4', init= 0, min= 0, max= 1 },
            { desc= 'Band1 (250Hz)', id= 'MFX_SPECT_BAND1', init= 32783, min= 32768, max= 32798 },
            { desc= 'Band2 (500Hz)', id= 'MFX_SPECT_BAND2', init= 32783, min= 32768, max= 32798 },
            { desc= 'Band3 (1000Hz)', id= 'MFX_SPECT_BAND3', init= 32783, min= 32768, max= 32798 },
            { desc= 'Band4 (1250Hz)', id= 'MFX_SPECT_BAND4', init= 32783, min= 32768, max= 32798 },
            { desc= 'Band5 (2000Hz)', id= 'MFX_SPECT_BAND5', init= 32783, min= 32768, max= 32798 },
            { desc= 'Band6 (3150Hz)', id= 'MFX_SPECT_BAND6', init= 32783, min= 32768, max= 32798 },
            { desc= 'Band7 (4000Hz)', id= 'MFX_SPECT_BAND7', init= 32783, min= 32768, max= 32798 },
            { desc= 'Band8 (8000Hz)', id= 'MFX_SPECT_BAND8', init= 32783, min= 32768, max= 32798 },
            { desc= 'Q', id= 'MFX_SPECT_WIDTH', init= 32768, min= 32768, max= 32772 },
            { desc= 'Level', id= 'MFX_SPECT_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'LOW BOOST',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_BOOST_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_BOOST_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_BOOST_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_BOOST_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Boost Frequency', id= 'MFX_BOOST_FREQ', init= 32772, min= 32768, max= 32776 },
            { desc= 'Boost Gain', id= 'MFX_BOOST_GAIN', init= 32774, min= 32768, max= 32780 },
            { desc= 'Boost Width', id= 'MFX_BOOST_WIDTH', init= 32768, min= 32768, max= 32770 },
            { desc= 'Low Gain', id= 'MFX_BOOST_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_BOOST_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_BOOST_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'STEP FILTER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_STEPFIL_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_STEPFIL_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_STEPFIL_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_STEPFIL_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Step 01', id= 'MFX_STEPFIL_STEP01', init= 32828, min= 32768, max= 32895 },
            { desc= 'Step 02', id= 'MFX_STEPFIL_STEP02', init= 32798, min= 32768, max= 32895 },
            { desc= 'Step 03', id= 'MFX_STEPFIL_STEP03', init= 32828, min= 32768, max= 32895 },
            { desc= 'Step 04', id= 'MFX_STEPFIL_STEP04', init= 32798, min= 32768, max= 32895 },
            { desc= 'Step 05', id= 'MFX_STEPFIL_STEP05', init= 32828, min= 32768, max= 32895 },
            { desc= 'Step 06', id= 'MFX_STEPFIL_STEP06', init= 32798, min= 32768, max= 32895 },
            { desc= 'Step 07', id= 'MFX_STEPFIL_STEP07', init= 32828, min= 32768, max= 32895 },
            { desc= 'Step 08', id= 'MFX_STEPFIL_STEP08', init= 32798, min= 32768, max= 32895 },
            { desc= 'Step 09', id= 'MFX_STEPFIL_STEP09', init= 32828, min= 32768, max= 32895 },
            { desc= 'Step 10', id= 'MFX_STEPFIL_STEP10', init= 32828, min= 32768, max= 32895 },
            { desc= 'Step 11', id= 'MFX_STEPFIL_STEP11', init= 32798, min= 32768, max= 32895 },
            { desc= 'Step 12', id= 'MFX_STEPFIL_STEP12', init= 32828, min= 32768, max= 32895 },
            { desc= 'Step 13', id= 'MFX_STEPFIL_STEP13', init= 32828, min= 32768, max= 32895 },
            { desc= 'Step 14', id= 'MFX_STEPFIL_STEP14', init= 32798, min= 32768, max= 32895 },
            { desc= 'Step 15', id= 'MFX_STEPFIL_STEP15', init= 32828, min= 32768, max= 32895 },
            { desc= 'Step 16', id= 'MFX_STEPFIL_STEP16', init= 32798, min= 32768, max= 32895 },
            { desc= 'Rate (sync sw)', id= 'MFX_STEPFIL_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_STEPFIL_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_STEPFIL_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Attack', id= 'MFX_STEPFIL_ATTACK', init= 32818, min= 32768, max= 32895 },
            { desc= 'Filter Type', id= 'MFX_STEPFIL_FILT_TYPE', init= 32770, min= 32768, max= 32771 },
            { desc= 'Filter Slope', id= 'MFX_STEPFIL_FILT_SLOPE', init= 32770, min= 32768, max= 32770 },
            { desc= 'Filter Resonance', id= 'MFX_STEPFIL_FILT_RESO', init= 32808, min= 32768, max= 32895 },
            { desc= 'Filter Gain', id= 'MFX_STEPFIL_FILT_GAIN', init= 32768, min= 32768, max= 32780 },
            { desc= 'Level', id= 'MFX_STEPFIL_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'ENHANCER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_EH_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_EH_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_EH_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_EH_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Sens', id= 'MFX_EH_SENS', init= 32832, min= 32768, max= 32895 },
            { desc= 'Mix', id= 'MFX_EH_MIX', init= 32832, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_EH_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_EH_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_EH_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'AUTO WAH',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_WAH_CTRL_ASGN1', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 2', id= 'MFX_WAH_CTRL_ASGN2', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 3', id= 'MFX_WAH_CTRL_ASGN3', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 4', id= 'MFX_WAH_CTRL_ASGN4', init= 0, min= 0, max= 5 },
            { desc= 'Filter Type', id= 'MFX_WAH_MODE', init= 32769, min= 32768, max= 32769 },
            { desc= 'Manual', id= 'MFX_WAH_MANUAL', init= 32828, min= 32768, max= 32895 },
            { desc= 'Peak', id= 'MFX_WAH_PEAK', init= 32808, min= 32768, max= 32895 },
            { desc= 'Sens', id= 'MFX_WAH_SENS', init= 32768, min= 32768, max= 32895 },
            { desc= 'Polarity', id= 'MFX_WAH_POLARITY', init= 32768, min= 32768, max= 32769 },
            { desc= 'Rate (sync sw)', id= 'MFX_WAH_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_WAH_RATE_HZ', init= 32808, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_WAH_RATE_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_WAH_DEPTH', init= 32828, min= 32768, max= 32895 },
            { desc= 'Phase', id= 'MFX_WAH_PHS', init= 32768, min= 32768, max= 32858 },
            { desc= 'Low Gain', id= 'MFX_WAH_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_WAH_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_WAH_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'HUMANIZER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_FORMANT_CTRL_ASGN1', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 2', id= 'MFX_FORMANT_CTRL_ASGN2', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 3', id= 'MFX_FORMANT_CTRL_ASGN3', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 4', id= 'MFX_FORMANT_CTRL_ASGN4', init= 0, min= 0, max= 5 },
            { desc= 'Drive Sw', id= 'MFX_FORMANT_DRIVE_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Drive', id= 'MFX_FORMANT_DRV', init= 32895, min= 32768, max= 32895 },
            { desc= 'Vowel1', id= 'MFX_FORMANT_VOWEL1', init= 32772, min= 32768, max= 32772 },
            { desc= 'Vowel2', id= 'MFX_FORMANT_VOWEL2', init= 32768, min= 32768, max= 32772 },
            { desc= 'Rate (sync sw)', id= 'MFX_FORMANT_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_FORMANT_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_FORMANT_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_FORMANT_DEPTH', init= 32895, min= 32768, max= 32895 },
            { desc= 'Input Sync Sw', id= 'MFX_FORMANT_KS_SW', init= 32768, min= 32768, max= 32769 },
            { desc= 'Input Sync Threshold', id= 'MFX_FORMANT_KS_THRE', init= 32828, min= 32768, max= 32895 },
            { desc= 'Manual', id= 'MFX_FORMANT_MANUAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Low Gain', id= 'MFX_FORMANT_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_FORMANT_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Pan', id= 'MFX_FORMANT_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_FORMANT_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'SPEAKER SIMULATOR',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_SPSIM_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_SPSIM_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_SPSIM_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_SPSIM_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Speaker Type', id= 'MFX_SPSIM_SP_TYPE', init= 32774, min= 32768, max= 32783 },
            { desc= 'Mic Setting', id= 'MFX_SPSIM_MIC_SETTING', init= 32769, min= 32768, max= 32770 },
            { desc= 'Mic Level', id= 'MFX_SPSIM_MIC_LEVEL', init= 32895, min= 32768, max= 32895 },
            { desc= 'Direct Level', id= 'MFX_SPSIM_DIRECT_LEVEL', init= 32768, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_SPSIM_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'PHASER 1',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_STPH_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_STPH_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_STPH_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_STPH_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Mode', id= 'MFX_STPH_MODE', init= 32770, min= 32768, max= 32770 },
            { desc= 'Manual', id= 'MFX_STPH_MANUAL', init= 32832, min= 32768, max= 32895 },
            { desc= 'Rate (sync sw)', id= 'MFX_STPH_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_STPH_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_STPH_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_STPH_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Polarity', id= 'MFX_STPH_POLARITY', init= 32769, min= 32768, max= 32769 },
            { desc= 'Resonance', id= 'MFX_STPH_RESO', init= 32808, min= 32768, max= 32895 },
            { desc= 'Cross Feedback', id= 'MFX_STPH_XFBK', init= 32817, min= 32768, max= 32866 },
            { desc= 'Mix', id= 'MFX_STPH_MIX', init= 32895, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_STPH_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_STPH_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_STPH_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'PHASER 2',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_PHA2_CTRL_ASGN1', init= 0, min= 0, max= 1 },
            { desc= 'Control Assign 2', id= 'MFX_PHA2_CTRL_ASGN2', init= 0, min= 0, max= 1 },
            { desc= 'Control Assign 3', id= 'MFX_PHA2_CTRL_ASGN3', init= 0, min= 0, max= 1 },
            { desc= 'Control Assign 4', id= 'MFX_PHA2_CTRL_ASGN4', init= 0, min= 0, max= 1 },
            { desc= 'Rate', id= 'MFX_PHA2_RATE', init= 32818, min= 32768, max= 32868 },
            { desc= 'Color', id= 'MFX_PHA2_COLOR', init= 32768, min= 32768, max= 32769 },
            { desc= 'Low Gain', id= 'MFX_PHA2_LO_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_PHA2_HI_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_PHA2_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'PHASER 3',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_PHA3_CTRL_ASGN1', init= 0, min= 0, max= 1 },
            { desc= 'Control Assign 2', id= 'MFX_PHA3_CTRL_ASGN2', init= 0, min= 0, max= 1 },
            { desc= 'Control Assign 3', id= 'MFX_PHA3_CTRL_ASGN3', init= 0, min= 0, max= 1 },
            { desc= 'Control Assign 4', id= 'MFX_PHA3_CTRL_ASGN4', init= 0, min= 0, max= 1 },
            { desc= 'Speed', id= 'MFX_PHA3_SPEED', init= 32818, min= 32768, max= 32868 },
            { desc= 'Low Gain', id= 'MFX_PHA3_LO_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_PHA3_HI_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_PHA3_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'STEP PHASER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_STEPPH_CTRL_ASGN1', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 2', id= 'MFX_STEPPH_CTRL_ASGN2', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 3', id= 'MFX_STEPPH_CTRL_ASGN3', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 4', id= 'MFX_STEPPH_CTRL_ASGN4', init= 0, min= 0, max= 5 },
            { desc= 'Mode', id= 'MFX_STEPPH_MODE', init= 32770, min= 32768, max= 32770 },
            { desc= 'Manual', id= 'MFX_STEPPH_MANUAL', init= 32832, min= 32768, max= 32895 },
            { desc= 'Rate (sync sw)', id= 'MFX_STEPPH_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_STEPPH_RATE_HZ', init= 32798, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_STEPPH_RATE_NOTE', init= 32781, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_STEPPH_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Polarity', id= 'MFX_STEPPH_POLARITY', init= 32769, min= 32768, max= 32769 },
            { desc= 'Resonance', id= 'MFX_STEPPH_RESO', init= 32808, min= 32768, max= 32895 },
            { desc= 'Cross Feedback', id= 'MFX_STEPPH_XFBK', init= 32817, min= 32768, max= 32866 },
            { desc= 'Step Rate (sync sw)', id= 'MFX_STEPPH_S_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Step Rate (Hz)', id= 'MFX_STEPPH_S_RATE_HZ', init= 32848, min= 32769, max= 32968 },
            { desc= 'Step Rate (note)', id= 'MFX_STEPPH_S_RATE_NOTE', init= 32774, min= 32768, max= 32789 },
            { desc= 'Mix', id= 'MFX_STEPPH_MIX', init= 32895, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_STEPPH_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_STEPPH_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_STEPPH_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'MULTI STAGE PHASER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_MTPH_CTRL_ASGN1', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 2', id= 'MFX_MTPH_CTRL_ASGN2', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 3', id= 'MFX_MTPH_CTRL_ASGN3', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 4', id= 'MFX_MTPH_CTRL_ASGN4', init= 0, min= 0, max= 5 },
            { desc= 'Mode', id= 'MFX_MTPH_MODE', init= 32773, min= 32768, max= 32773 },
            { desc= 'Manual', id= 'MFX_MTPH_MANUAL', init= 32828, min= 32768, max= 32895 },
            { desc= 'Rate (sync sw)', id= 'MFX_MTPH_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_MTPH_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_MTPH_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_MTPH_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Resonance', id= 'MFX_MTPH_RESO', init= 32808, min= 32768, max= 32895 },
            { desc= 'Mix', id= 'MFX_MTPH_MIX', init= 32895, min= 32768, max= 32895 },
            { desc= 'Pan', id= 'MFX_MTPH_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_MTPH_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_MTPH_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_MTPH_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'INFINITE PHASER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_INFPH_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_INFPH_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_INFPH_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_INFPH_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Mode', id= 'MFX_INFPH_MODE', init= 32771, min= 32768, max= 32771 },
            { desc= 'Speed', id= 'MFX_INFPH_SPEED', init= 32908, min= 32768, max= 32968 },
            { desc= 'Resonance', id= 'MFX_INFPH_RESO', init= 32848, min= 32768, max= 32895 },
            { desc= 'Mix', id= 'MFX_INFPH_MIX', init= 32895, min= 32768, max= 32895 },
            { desc= 'Pan', id= 'MFX_INFPH_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_INFPH_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_INFPH_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_INFPH_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'RING MODULATOR',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_RING_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_RING_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_RING_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_RING_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Frequency', id= 'MFX_RING_FREQ', init= 32828, min= 32768, max= 32895 },
            { desc= 'Sens', id= 'MFX_RING_SENS', init= 32768, min= 32768, max= 32895 },
            { desc= 'Polarity', id= 'MFX_RING_POLARITY', init= 32768, min= 32768, max= 32769 },
            { desc= 'Low Gain', id= 'MFX_RING_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_RING_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_RING_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_RING_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'TREMOLO',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_TRE_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_TRE_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_TRE_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_TRE_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Mod Wave', id= 'MFX_TRE_MOD_WAVE', init= 32768, min= 32768, max= 32772 },
            { desc= 'Rate (sync sw)', id= 'MFX_TRE_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_TRE_RATE_HZ', init= 32848, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_TRE_RATE_NOTE', init= 32777, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_TRE_DEPTH', init= 32864, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_TRE_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_TRE_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_TRE_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'AUTO PAN',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_AUTOPAN_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_AUTOPAN_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_AUTOPAN_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_AUTOPAN_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Mod Wave', id= 'MFX_AUTOPAN_MOD_WAVE', init= 32768, min= 32768, max= 32772 },
            { desc= 'Rate (sync sw)', id= 'MFX_AUTOPAN_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_AUTOPAN_RATE_HZ', init= 32848, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_AUTOPAN_RATE_NOTE', init= 32777, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_AUTOPAN_DEPTH', init= 32864, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_AUTOPAN_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_AUTOPAN_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_AUTOPAN_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'SLICER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_SLICER_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_SLICER_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_SLICER_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_SLICER_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Step 01', id= 'MFX_SLICER_STEP01', init= 32895, min= 32768, max= 32895 },
            { desc= 'Step 02', id= 'MFX_SLICER_STEP02', init= 32768, min= 32768, max= 32895 },
            { desc= 'Step 03', id= 'MFX_SLICER_STEP03', init= 32798, min= 32768, max= 32895 },
            { desc= 'Step 04', id= 'MFX_SLICER_STEP04', init= 32895, min= 32768, max= 32895 },
            { desc= 'Step 05', id= 'MFX_SLICER_STEP05', init= 32895, min= 32768, max= 32895 },
            { desc= 'Step 06', id= 'MFX_SLICER_STEP06', init= 32768, min= 32768, max= 32895 },
            { desc= 'Step 07', id= 'MFX_SLICER_STEP07', init= 32798, min= 32768, max= 32895 },
            { desc= 'Step 08', id= 'MFX_SLICER_STEP08', init= 32768, min= 32768, max= 32895 },
            { desc= 'Step 09', id= 'MFX_SLICER_STEP09', init= 32895, min= 32768, max= 32895 },
            { desc= 'Step 10', id= 'MFX_SLICER_STEP10', init= 32768, min= 32768, max= 32895 },
            { desc= 'Step 11', id= 'MFX_SLICER_STEP11', init= 32798, min= 32768, max= 32895 },
            { desc= 'Step 12', id= 'MFX_SLICER_STEP12', init= 32895, min= 32768, max= 32895 },
            { desc= 'Step 13', id= 'MFX_SLICER_STEP13', init= 32768, min= 32768, max= 32895 },
            { desc= 'Step 14', id= 'MFX_SLICER_STEP14', init= 32768, min= 32768, max= 32895 },
            { desc= 'Step 15', id= 'MFX_SLICER_STEP15', init= 32798, min= 32768, max= 32895 },
            { desc= 'Step 16', id= 'MFX_SLICER_STEP16', init= 32768, min= 32768, max= 32895 },
            { desc= 'Rate (sync sw)', id= 'MFX_SLICER_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_SLICER_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_SLICER_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Attack', id= 'MFX_SLICER_ATTACK', init= 32818, min= 32768, max= 32895 },
            { desc= 'Input Sync Sw', id= 'MFX_SLICER_RESET_SW', init= 32768, min= 32768, max= 32769 },
            { desc= 'Input Sync Threshold', id= 'MFX_SLICER_RESET_THRE', init= 32828, min= 32768, max= 32895 },
            { desc= 'Mode', id= 'MFX_SLICER_MODE', init= 32768, min= 32768, max= 32769 },
            { desc= 'Shuffle', id= 'MFX_SLICER_SHUFFLE', init= 32768, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_SLICER_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'ROTARY 1',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_ROT_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_ROT_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_ROT_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_ROT_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Speed', id= 'MFX_ROT_SPEED', init= 32768, min= 32768, max= 32769 },
            { desc= 'Woofer Slow Speed', id= 'MFX_ROT_WF_SLOW_SPEED', init= 32808, min= 32769, max= 32968 },
            { desc= 'Woofer Fast Speed', id= 'MFX_ROT_WF_FAST_SPEED', init= 32928, min= 32769, max= 32968 },
            { desc= 'Woofer Acceleration', id= 'MFX_ROT_WF_ACCEL', init= 32778, min= 32768, max= 32783 },
            { desc= 'Woofer Level', id= 'MFX_ROT_WF_LEVEL', init= 32895, min= 32768, max= 32895 },
            { desc= 'Tweeter Slow Speed', id= 'MFX_ROT_TW_SLOW_SPEED', init= 32808, min= 32769, max= 32968 },
            { desc= 'Tweeter Fast Speed', id= 'MFX_ROT_TW_FAST_SPEED', init= 32928, min= 32769, max= 32968 },
            { desc= 'Tweeter Acceleration', id= 'MFX_ROT_TW_ACCEL', init= 32778, min= 32768, max= 32783 },
            { desc= 'Tweeter Level', id= 'MFX_ROT_TW_LEVEL', init= 32895, min= 32768, max= 32895 },
            { desc= 'Separation', id= 'MFX_ROT_SEPA', init= 32895, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_ROT_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'ROTARY 2',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_VKROT_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_VKROT_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_VKROT_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_VKROT_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Speed', id= 'MFX_VKROT_SPEED', init= 32768, min= 32768, max= 32769 },
            { desc= 'Brake', id= 'MFX_VKROT_BRAKE', init= 32768, min= 32768, max= 32769 },
            { desc= 'Woofer Slow Speed', id= 'MFX_VKROT_WF_SLOW_SPEED', init= 32808, min= 32769, max= 32968 },
            { desc= 'Woofer Fast Speed', id= 'MFX_VKROT_WF_FAST_SPEED', init= 32928, min= 32769, max= 32968 },
            { desc= 'Woofer Trans Up', id= 'MFX_VKROT_WF_TRANS_UP', init= 32832, min= 32768, max= 32895 },
            { desc= 'Woofer Trans Down', id= 'MFX_VKROT_WF_TRANS_DW', init= 32832, min= 32768, max= 32895 },
            { desc= 'Woofer Level', id= 'MFX_VKROT_WF_LEVEL', init= 32895, min= 32768, max= 32895 },
            { desc= 'Tweeter Slow Speed', id= 'MFX_VKROT_TW_SLOW_SPEED', init= 32808, min= 32769, max= 32968 },
            { desc= 'Tweeter Fast Speed', id= 'MFX_VKROT_TW_FAST_SPEED', init= 32928, min= 32769, max= 32968 },
            { desc= 'Tweeter Trans Up', id= 'MFX_VKROT_TW_TRANS_UP', init= 32832, min= 32768, max= 32895 },
            { desc= 'Tweeter Trans Down', id= 'MFX_VKROT_TW_TRANS_DW', init= 32832, min= 32768, max= 32895 },
            { desc= 'Tweeter Level', id= 'MFX_VKROT_TW_LEVEL', init= 32895, min= 32768, max= 32895 },
            { desc= 'Spread', id= 'MFX_VKROT_SPREAD', init= 32778, min= 32768, max= 32778 },
            { desc= 'Low Gain', id= 'MFX_VKROT_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_VKROT_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_VKROT_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'ROTARY 3',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_ROT3_CTRL_ASGN1', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 2', id= 'MFX_ROT3_CTRL_ASGN2', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 3', id= 'MFX_ROT3_CTRL_ASGN3', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 4', id= 'MFX_ROT3_CTRL_ASGN4', init= 0, min= 0, max= 5 },
            { desc= 'Speed', id= 'MFX_ROT3_SPEED', init= 32768, min= 32768, max= 32769 },
            { desc= 'Brake', id= 'MFX_ROT3_BRAKE', init= 32768, min= 32768, max= 32769 },
            { desc= 'Woofer Slow Speed', id= 'MFX_ROT3_WF_SLOW_SPEED', init= 32808, min= 32769, max= 32968 },
            { desc= 'Woofer Fast Speed', id= 'MFX_ROT3_WF_FAST_SPEED', init= 32928, min= 32769, max= 32968 },
            { desc= 'Woofer Trans Up', id= 'MFX_ROT3_WF_TRANS_UP', init= 32832, min= 32768, max= 32895 },
            { desc= 'Woofer Trans Down', id= 'MFX_ROT3_WF_TRANS_DW', init= 32832, min= 32768, max= 32895 },
            { desc= 'Woofer Level', id= 'MFX_ROT3_WF_LEVEL', init= 32895, min= 32768, max= 32895 },
            { desc= 'Tweeter Slow Speed', id= 'MFX_ROT3_TW_SLOW_SPEED', init= 32808, min= 32769, max= 32968 },
            { desc= 'Tweeter Fast Speed', id= 'MFX_ROT3_TW_FAST_SPEED', init= 32928, min= 32769, max= 32968 },
            { desc= 'Tweeter Trans Up', id= 'MFX_ROT3_TW_TRANS_UP', init= 32832, min= 32768, max= 32895 },
            { desc= 'Tweeter Trans Down', id= 'MFX_ROT3_TW_TRANS_DW', init= 32832, min= 32768, max= 32895 },
            { desc= 'Tweeter Level', id= 'MFX_ROT3_TW_LEVEL', init= 32895, min= 32768, max= 32895 },
            { desc= 'Spread', id= 'MFX_ROT3_SPREAD', init= 32778, min= 32768, max= 32778 },
            { desc= 'Low Gain', id= 'MFX_ROT3_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_ROT3_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_ROT3_LEVEL', init= 32895, min= 32768, max= 32895 },
            { desc= 'OD Switch', id= 'MFX_ROT3_OD_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'OD Gain', id= 'MFX_ROT3_OD_GAIN', init= 32848, min= 32768, max= 32895 },
            { desc= 'OD Drive', id= 'MFX_ROT3_OD_DRIVE', init= 32868, min= 32768, max= 32895 },
            { desc= 'OD Level', id= 'MFX_ROT3_OD_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'CHORUS',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_STCHO_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_STCHO_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_STCHO_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_STCHO_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Filter Type', id= 'MFX_STCHO_FILT', init= 32770, min= 32768, max= 32770 },
            { desc= 'Cutoff Freq', id= 'MFX_STCHO_SPLT', init= 32774, min= 32768, max= 32784 },
            { desc= 'Pre Delay', id= 'MFX_STCHO_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Rate (sync sw)', id= 'MFX_STCHO_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_STCHO_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_STCHO_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_STCHO_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'Phase', id= 'MFX_STCHO_PHS', init= 32858, min= 32768, max= 32858 },
            { desc= 'Low Gain', id= 'MFX_STCHO_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_STCHO_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_STCHO_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_STCHO_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'FLANGER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_STFLG_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_STFLG_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_STFLG_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_STFLG_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Filter Type', id= 'MFX_STFLG_FILT', init= 32770, min= 32768, max= 32770 },
            { desc= 'Cutoff Freq', id= 'MFX_STFLG_SPLT', init= 32774, min= 32768, max= 32784 },
            { desc= 'Pre Delay', id= 'MFX_STFLG_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Rate (sync sw)', id= 'MFX_STFLG_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_STFLG_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_STFLG_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_STFLG_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Phase', id= 'MFX_STFLG_PHS', init= 32858, min= 32768, max= 32858 },
            { desc= 'Feedback', id= 'MFX_STFLG_FBK', init= 32847, min= 32768, max= 32866 },
            { desc= 'Low Gain', id= 'MFX_STFLG_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_STFLG_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_STFLG_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_STFLG_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'STEP FLANGER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_STEPFLG_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_STEPFLG_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_STEPFLG_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_STEPFLG_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Filter Type', id= 'MFX_STEPFLG_FILT', init= 32770, min= 32768, max= 32770 },
            { desc= 'Cutoff Freq', id= 'MFX_STEPFLG_SPLT', init= 32774, min= 32768, max= 32784 },
            { desc= 'Pre Delay', id= 'MFX_STEPFLG_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Rate (sync sw)', id= 'MFX_STEPFLG_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_STEPFLG_RATE_HZ', init= 32798, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_STEPFLG_RATE_NOTE', init= 32781, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_STEPFLG_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Phase', id= 'MFX_STEPFLG_PHS', init= 32858, min= 32768, max= 32858 },
            { desc= 'Feedback', id= 'MFX_STEPFLG_FBK', init= 32847, min= 32768, max= 32866 },
            { desc= 'Step Rate (sync sw)', id= 'MFX_STEPFLG_S_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Step Rate (Hz)', id= 'MFX_STEPFLG_S_RATE_HZ', init= 32848, min= 32769, max= 32968 },
            { desc= 'Step Rate (note)', id= 'MFX_STEPFLG_S_RATE_NOTE', init= 32774, min= 32768, max= 32789 },
            { desc= 'Low Gain', id= 'MFX_STEPFLG_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_STEPFLG_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_STEPFLG_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_STEPFLG_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'HEXA-CHORUS',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_HEXCHO_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_HEXCHO_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_HEXCHO_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_HEXCHO_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Pre Delay', id= 'MFX_HEXCHO_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Rate (sync sw)', id= 'MFX_HEXCHO_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_HEXCHO_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_HEXCHO_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_HEXCHO_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'Pre Delay Deviation', id= 'MFX_HEXCHO_DLY_DEV', init= 32768, min= 32768, max= 32788 },
            { desc= 'Depth Deviation', id= 'MFX_HEXCHO_DEPTH_DEV', init= 32788, min= 32768, max= 32808 },
            { desc= 'Pan Deviation', id= 'MFX_HEXCHO_PAN_DEV', init= 32788, min= 32768, max= 32788 },
            { desc= 'Balance', id= 'MFX_HEXCHO_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_HEXCHO_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'TREMOLO CHORUS',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_TRECHO_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_TRECHO_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_TRECHO_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_TRECHO_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Pre Delay', id= 'MFX_TRECHO_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Chorus Rate (sync sw)', id= 'MFX_TRECHO_CHO_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Chorus Rate (Hz)', id= 'MFX_TRECHO_CHO_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Chorus Rate (note)', id= 'MFX_TRECHO_CHO_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Chorus Depth', id= 'MFX_TRECHO_CHO_DEPTH', init= 32818, min= 32768, max= 32895 },
            { desc= 'Tremolo Rate (sync sw)', id= 'MFX_TRECHO_TRE_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Tremolo Rate (Hz)', id= 'MFX_TRECHO_TRE_RATE_HZ', init= 32808, min= 32769, max= 32968 },
            { desc= 'Tremolo Rate (note)', id= 'MFX_TRECHO_TRE_RATE_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Tremolo Separation', id= 'MFX_TRECHO_TRE_SEPA', init= 32895, min= 32768, max= 32895 },
            { desc= 'Tremolo Phase', id= 'MFX_TRECHO_TRE_PHS', init= 32858, min= 32768, max= 32858 },
            { desc= 'Balance', id= 'MFX_TRECHO_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_TRECHO_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'SPACE-D',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_SPACED_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_SPACED_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_SPACED_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_SPACED_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Pre Delay', id= 'MFX_SPACED_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Rate (sync sw)', id= 'MFX_SPACED_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_SPACED_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_SPACED_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_SPACED_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'Phase', id= 'MFX_SPACED_PHS', init= 32858, min= 32768, max= 32858 },
            { desc= 'Low Gain', id= 'MFX_SPACED_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_SPACED_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_SPACED_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_SPACED_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'OVERDRIVE',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_VSOD_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_VSOD_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_VSOD_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_VSOD_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Drive', id= 'MFX_VSOD_DRV', init= 32895, min= 32768, max= 32895 },
            { desc= 'Tone', id= 'MFX_VSOD_TONE', init= 32818, min= 32768, max= 32895 },
            { desc= 'Amp Sw', id= 'MFX_VSOD_AMP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Amp Type', id= 'MFX_VSOD_AMP_TYPE', init= 32768, min= 32768, max= 32771 },
            { desc= 'Low Gain', id= 'MFX_VSOD_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_VSOD_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Pan', id= 'MFX_VSOD_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_VSOD_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'DISTORTION',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_VSDST_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_VSDST_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_VSDST_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_VSDST_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Drive', id= 'MFX_VSDST_DRV', init= 32895, min= 32768, max= 32895 },
            { desc= 'Tone', id= 'MFX_VSDST_TONE', init= 32818, min= 32768, max= 32895 },
            { desc= 'Amp Sw', id= 'MFX_VSDST_AMP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Amp Type', id= 'MFX_VSDST_AMP_TYPE', init= 32771, min= 32768, max= 32771 },
            { desc= 'Low Gain', id= 'MFX_VSDST_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_VSDST_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Pan', id= 'MFX_VSDST_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_VSDST_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'GUITAR AMP SIMULATOR',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_GAMPSIM_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_GAMPSIM_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_GAMPSIM_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_GAMPSIM_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Pre Amp Sw', id= 'MFX_GAMPSIM_AMP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Pre Amp Type', id= 'MFX_GAMPSIM_AMP_TYPE', init= 32769, min= 32768, max= 32781 },
            { desc= 'Pre Amp Volume', id= 'MFX_GAMPSIM_AMP_VOLUME', init= 32848, min= 32768, max= 32895 },
            { desc= 'Pre Amp Master', id= 'MFX_GAMPSIM_AMP_MASTER', init= 32868, min= 32768, max= 32895 },
            { desc= 'Pre Amp Gain', id= 'MFX_GAMPSIM_AMP_GAIN', init= 32769, min= 32768, max= 32770 },
            { desc= 'Pre Amp Bass', id= 'MFX_GAMPSIM_AMP_BASS', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pre Amp Middle', id= 'MFX_GAMPSIM_AMP_MIDDLE', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pre Amp Treble', id= 'MFX_GAMPSIM_AMP_TREBLE', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pre Amp Presence', id= 'MFX_GAMPSIM_AMP_PRESENCE', init= 32768, min= 32768, max= 32895 },
            { desc= 'Pre Amp Bright', id= 'MFX_GAMPSIM_AMP_BRIGHT', init= 32768, min= 32768, max= 32769 },
            { desc= 'Speaker Sw', id= 'MFX_GAMPSIM_SP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Speaker Type', id= 'MFX_GAMPSIM_SP_TYPE', init= 32772, min= 32768, max= 32783 },
            { desc= 'Mic Setting', id= 'MFX_GAMPSIM_MIC_SETTING', init= 32769, min= 32768, max= 32770 },
            { desc= 'Mic Level', id= 'MFX_GAMPSIM_MIC_LEVEL', init= 32895, min= 32768, max= 32895 },
            { desc= 'Direct Level', id= 'MFX_GAMPSIM_DIRECT_LEVEL', init= 32768, min= 32768, max= 32895 },
            { desc= 'Pan', id= 'MFX_GAMPSIM_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_GAMPSIM_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'COMPRESSOR',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_COMP_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_COMP_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_COMP_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_COMP_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Attack', id= 'MFX_COMP_ATK', init= 32788, min= 32768, max= 32895 },
            { desc= 'Threshold', id= 'MFX_COMP_THRES', init= 32832, min= 32768, max= 32895 },
            { desc= 'Post Gain', id= 'MFX_COMP_GAIN', init= 32774, min= 32768, max= 32786 },
            { desc= 'Low Gain', id= 'MFX_COMP_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_COMP_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_COMP_LEVEL', init= 32895, min= 32768, max= 32895 },
            { desc= '', id= 'MFX_COMP_RATIO', init= 32768, min= 32768, max= 32770 }
        }
    },

    {
        desc= 'LIMITER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_LIMIT_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_LIMIT_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_LIMIT_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_LIMIT_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Release', id= 'MFX_LIMIT_RELS', init= 32800, min= 32768, max= 32895 },
            { desc= 'Threshold', id= 'MFX_LIMIT_THRES', init= 32832, min= 32768, max= 32895 },
            { desc= 'Ratio', id= 'MFX_LIMIT_RATIO', init= 32770, min= 32768, max= 32771 },
            { desc= 'Post Gain', id= 'MFX_LIMIT_GAIN', init= 32774, min= 32768, max= 32786 },
            { desc= 'Low Gain', id= 'MFX_LIMIT_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_LIMIT_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_LIMIT_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'GATE',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_GATE_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_GATE_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_GATE_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_GATE_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Threshold', id= 'MFX_GATE_THRES', init= 32838, min= 32768, max= 32895 },
            { desc= 'Mode', id= 'MFX_GATE_MODE', init= 32768, min= 32768, max= 32769 },
            { desc= 'Attack', id= 'MFX_GATE_ATK', init= 32776, min= 32768, max= 32895 },
            { desc= 'Hold', id= 'MFX_GATE_HOLD', init= 32768, min= 32768, max= 32895 },
            { desc= 'Release', id= 'MFX_GATE_RELS', init= 32784, min= 32768, max= 32895 },
            { desc= 'Balance', id= 'MFX_GATE_BAL', init= 32868, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_GATE_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_STDLY_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_STDLY_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_STDLY_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_STDLY_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Delay Left (sync sw)', id= 'MFX_STDLY_DLY_L_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Left (msec)', id= 'MFX_STDLY_DLY_L_MSEC', init= 33368, min= 32768, max= 34068 },
            { desc= 'Delay Left (note)', id= 'MFX_STDLY_DLY_L_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Delay Right (sync sw)', id= 'MFX_STDLY_DLY_R_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Right (msec)', id= 'MFX_STDLY_DLY_R_MSEC', init= 33368, min= 32768, max= 34068 },
            { desc= 'Delay Right (note)', id= 'MFX_STDLY_DLY_R_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Phase Left', id= 'MFX_STDLY_PHS_L', init= 32768, min= 32768, max= 32769 },
            { desc= 'Phase Right', id= 'MFX_STDLY_PHS_R', init= 32768, min= 32768, max= 32769 },
            { desc= 'Feedback Mode', id= 'MFX_STDLY_FBK_MODE', init= 32768, min= 32768, max= 32769 },
            { desc= 'Feedback', id= 'MFX_STDLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'HF Damp', id= 'MFX_STDLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Low Gain', id= 'MFX_STDLY_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_STDLY_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_STDLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_STDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'MODULATION DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_MODDLY_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_MODDLY_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_MODDLY_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_MODDLY_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Delay Left (sync sw)', id= 'MFX_MODDLY_DLY_L_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Left (msec)', id= 'MFX_MODDLY_DLY_L_MSEC', init= 33368, min= 32768, max= 34068 },
            { desc= 'Delay Left (note)', id= 'MFX_MODDLY_DLY_L_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Delay Right (sync sw)', id= 'MFX_MODDLY_DLY_R_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Right (msec)', id= 'MFX_MODDLY_DLY_R_MSEC', init= 33368, min= 32768, max= 34068 },
            { desc= 'Delay Right (note)', id= 'MFX_MODDLY_DLY_R_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Feedback Mode', id= 'MFX_MODDLY_FBK_MODE', init= 32768, min= 32768, max= 32769 },
            { desc= 'Feedback', id= 'MFX_MODDLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'HF Damp', id= 'MFX_MODDLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Rate (sync sw)', id= 'MFX_MODDLY_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Rate (Hz)', id= 'MFX_MODDLY_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Rate (note)', id= 'MFX_MODDLY_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Depth', id= 'MFX_MODDLY_DEPTH', init= 32788, min= 32768, max= 32895 },
            { desc= 'Phase', id= 'MFX_MODDLY_PHS', init= 32858, min= 32768, max= 32858 },
            { desc= 'Low Gain', id= 'MFX_MODDLY_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_MODDLY_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_MODDLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_MODDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= '3TAP PAN DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_TRIDLY_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_TRIDLY_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_TRIDLY_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_TRIDLY_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Delay Left (sync sw)', id= 'MFX_TRIDLY_DLY_L_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Left (msec)', id= 'MFX_TRIDLY_DLY_L_MSEC', init= 33168, min= 32768, max= 35368 },
            { desc= 'Delay Left (note)', id= 'MFX_TRIDLY_DLY_L_NOTE', init= 32778, min= 32768, max= 32789 },
            { desc= 'Delay Right (sync sw)', id= 'MFX_TRIDLY_DLY_R_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Right (msec)', id= 'MFX_TRIDLY_DLY_R_MSEC', init= 33568, min= 32768, max= 35368 },
            { desc= 'Delay Right (note)', id= 'MFX_TRIDLY_DLY_R_NOTE', init= 32781, min= 32768, max= 32789 },
            { desc= 'Delay Center (sync sw)', id= 'MFX_TRIDLY_DLY_C_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Center (msec)', id= 'MFX_TRIDLY_DLY_C_MSEC', init= 33968, min= 32768, max= 35368 },
            { desc= 'Delay Center (note)', id= 'MFX_TRIDLY_DLY_C_NOTE', init= 32783, min= 32768, max= 32789 },
            { desc= 'Center Feedback', id= 'MFX_TRIDLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'HF Damp', id= 'MFX_TRIDLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Left Level ', id= 'MFX_TRIDLY_LEVEL_L', init= 32895, min= 32768, max= 32895 },
            { desc= 'Right Level', id= 'MFX_TRIDLY_LEVEL_R', init= 32895, min= 32768, max= 32895 },
            { desc= 'Center Level', id= 'MFX_TRIDLY_LEVEL_C', init= 32895, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_TRIDLY_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_TRIDLY_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_TRIDLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_TRIDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= '4TAP PAN DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_QUADDLY_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_QUADDLY_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_QUADDLY_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_QUADDLY_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Delay 1 Time (sync sw)', id= 'MFX_QUADDLY_DLY1_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay 1 Time (msec)', id= 'MFX_QUADDLY_DLY1_MSEC', init= 33968, min= 32768, max= 35368 },
            { desc= 'Delay 1 Time (note)', id= 'MFX_QUADDLY_DLY1_NOTE', init= 32783, min= 32768, max= 32789 },
            { desc= 'Delay 2 Time (sync sw)', id= 'MFX_QUADDLY_DLY2_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay 2 Time (msec)', id= 'MFX_QUADDLY_DLY2_MSEC', init= 33668, min= 32768, max= 35368 },
            { desc= 'Delay 2 Time (note)', id= 'MFX_QUADDLY_DLY2_NOTE', init= 32782, min= 32768, max= 32789 },
            { desc= 'Delay 3 Time (sync sw)', id= 'MFX_QUADDLY_DLY3_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay 3 Time (msec)', id= 'MFX_QUADDLY_DLY3_MSEC', init= 33368, min= 32768, max= 35368 },
            { desc= 'Delay 3 Time (note)', id= 'MFX_QUADDLY_DLY3_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Delay 4 Time (sync sw)', id= 'MFX_QUADDLY_DLY4_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay 4 Time (msec)', id= 'MFX_QUADDLY_DLY4_MSEC', init= 33068, min= 32768, max= 35368 },
            { desc= 'Delay 4 Time (note)', id= 'MFX_QUADDLY_DLY4_NOTE', init= 32777, min= 32768, max= 32789 },
            { desc= 'Delay 1 Feedback', id= 'MFX_QUADDLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'HF Damp', id= 'MFX_QUADDLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Delay 1 Level', id= 'MFX_QUADDLY_LEVEL1', init= 32895, min= 32768, max= 32895 },
            { desc= 'Delay 2 Level', id= 'MFX_QUADDLY_LEVEL2', init= 32895, min= 32768, max= 32895 },
            { desc= 'Delay 3 Level', id= 'MFX_QUADDLY_LEVEL3', init= 32895, min= 32768, max= 32895 },
            { desc= 'Delay 4 Level', id= 'MFX_QUADDLY_LEVEL4', init= 32895, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_QUADDLY_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_QUADDLY_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_QUADDLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_QUADDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'MULTI TAP DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_MTDLY_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_MTDLY_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_MTDLY_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_MTDLY_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Delay 1 Time (sync sw)', id= 'MFX_MTDLY_DLY1_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay 1 Time (msec)', id= 'MFX_MTDLY_DLY1_MSEC', init= 33968, min= 32768, max= 35368 },
            { desc= 'Delay 1 Time (note)', id= 'MFX_MTDLY_DLY1_NOTE', init= 32783, min= 32768, max= 32789 },
            { desc= 'Delay 2 Time (sync sw)', id= 'MFX_MTDLY_DLY2_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay 2 Time (msec)', id= 'MFX_MTDLY_DLY2_MSEC', init= 33668, min= 32768, max= 35368 },
            { desc= 'Delay 2 Time (note)', id= 'MFX_MTDLY_DLY2_NOTE', init= 32782, min= 32768, max= 32789 },
            { desc= 'Delay 3 Time (sync sw)', id= 'MFX_MTDLY_DLY3_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay 3 Time (msec)', id= 'MFX_MTDLY_DLY3_MSEC', init= 33368, min= 32768, max= 35368 },
            { desc= 'Delay 3 Time (note)', id= 'MFX_MTDLY_DLY3_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Delay 4 Time (sync sw)', id= 'MFX_MTDLY_DLY4_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay 4 Time (msec)', id= 'MFX_MTDLY_DLY4_MSEC', init= 33068, min= 32768, max= 35368 },
            { desc= 'Delay 4 Time (note)', id= 'MFX_MTDLY_DLY4_NOTE', init= 32777, min= 32768, max= 32789 },
            { desc= 'Delay 1 Feedback', id= 'MFX_MTDLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'HF Damp', id= 'MFX_MTDLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Delay 1 Pan', id= 'MFX_MTDLY_PAN1', init= 32768, min= 32768, max= 32895 },
            { desc= 'Delay 2 Pan', id= 'MFX_MTDLY_PAN2', init= 32895, min= 32768, max= 32895 },
            { desc= 'Delay 3 Pan', id= 'MFX_MTDLY_PAN3', init= 32800, min= 32768, max= 32895 },
            { desc= 'Delay 4 Pan', id= 'MFX_MTDLY_PAN4', init= 32864, min= 32768, max= 32895 },
            { desc= 'Delay 1 Level', id= 'MFX_MTDLY_LEVEL1', init= 32895, min= 32768, max= 32895 },
            { desc= 'Delay 2 Level', id= 'MFX_MTDLY_LEVEL2', init= 32895, min= 32768, max= 32895 },
            { desc= 'Delay 3 Level', id= 'MFX_MTDLY_LEVEL3', init= 32895, min= 32768, max= 32895 },
            { desc= 'Delay 4 Level', id= 'MFX_MTDLY_LEVEL4', init= 32895, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_MTDLY_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_MTDLY_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_MTDLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_MTDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'REVERSE DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_RVSDLY_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_RVSDLY_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_RVSDLY_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_RVSDLY_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Threshold', id= 'MFX_RVSDLY_THRESHOLD', init= 32798, min= 32768, max= 32895 },
            { desc= 'Rev Delay Time (sync sw)', id= 'MFX_RVSDLY_DLYR_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Rev Delay Time (msec)', id= 'MFX_RVSDLY_DLYR_MSEC', init= 33368, min= 32768, max= 34068 },
            { desc= 'Rev Delay Time (note)', id= 'MFX_RVSDLY_DLYR_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Rev Delay Feedback', id= 'MFX_RVSDLY_DLYR_FBK', init= 32817, min= 32768, max= 32866 },
            { desc= 'Rev Delay HF Damp', id= 'MFX_RVSDLY_DLYR_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Rev Delay Pan', id= 'MFX_RVSDLY_DLYR_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Rev Delay Level', id= 'MFX_RVSDLY_DLYR_LVL', init= 32895, min= 32768, max= 32895 },
            { desc= 'Delay 1 Time (sync sw)', id= 'MFX_RVSDLY_DLY1_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay 1 Time (msec)', id= 'MFX_RVSDLY_DLY1_MSEC', init= 33068, min= 32768, max= 34068 },
            { desc= 'Delay 1 Time (note)', id= 'MFX_RVSDLY_DLY1_NOTE', init= 32777, min= 32768, max= 32789 },
            { desc= 'Delay 2 Time (sync sw)', id= 'MFX_RVSDLY_DLY2_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay 2 Time (msec)', id= 'MFX_RVSDLY_DLY2_MSEC', init= 33368, min= 32768, max= 34068 },
            { desc= 'Delay 2 Time (note)', id= 'MFX_RVSDLY_DLY2_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Delay 3 Time (sync sw)', id= 'MFX_RVSDLY_DLY3_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay 3 Time (msec)', id= 'MFX_RVSDLY_DLY3_MSEC', init= 33368, min= 32768, max= 34068 },
            { desc= 'Delay 3 Time (note)', id= 'MFX_RVSDLY_DLY3_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Delay 3 Feedback', id= 'MFX_RVSDLY_DLY3_FBK', init= 32817, min= 32768, max= 32866 },
            { desc= 'Delay HF Damp', id= 'MFX_RVSDLY_DLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Delay 1 Pan', id= 'MFX_RVSDLY_DLY1_PAN', init= 32768, min= 32768, max= 32895 },
            { desc= 'Delay 2 Pan', id= 'MFX_RVSDLY_DLY2_PAN', init= 32895, min= 32768, max= 32895 },
            { desc= 'Delay 1 Level', id= 'MFX_RVSDLY_DLY1_LVL', init= 32768, min= 32768, max= 32895 },
            { desc= 'Delay 2 Level', id= 'MFX_RVSDLY_DLY2_LVL', init= 32768, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_RVSDLY_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_RVSDLY_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_RVSDLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_RVSDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'TIME CTRL DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_STTMDLY_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_STTMDLY_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_STTMDLY_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_STTMDLY_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Delay Time (sync sw)', id= 'MFX_STTMDLY_DLY_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Delay Time (msec)', id= 'MFX_STTMDLY_DLY_MSEC', init= 33368, min= 32768, max= 34068 },
            { desc= 'Delay Time (note)', id= 'MFX_STTMDLY_DLY_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Acceleration', id= 'MFX_STTMDLY_ACCEL', init= 32778, min= 32768, max= 32783 },
            { desc= 'Feedback', id= 'MFX_STTMDLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'HF Damp', id= 'MFX_STTMDLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Low Gain', id= 'MFX_STTMDLY_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_STTMDLY_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_STTMDLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_STTMDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'LOFI COMPRESS',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_LFCOMP_CTRL_ASGN1', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 2', id= 'MFX_LFCOMP_CTRL_ASGN2', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 3', id= 'MFX_LFCOMP_CTRL_ASGN3', init= 0, min= 0, max= 2 },
            { desc= 'Control Assign 4', id= 'MFX_LFCOMP_CTRL_ASGN4', init= 0, min= 0, max= 2 },
            { desc= 'Pre Filter Type', id= 'MFX_LFCOMP_PRE_FILT', init= 32769, min= 32768, max= 32773 },
            { desc= 'LoFi Type', id= 'MFX_LFCOMP_LOFI_TYPE', init= 32772, min= 32768, max= 32776 },
            { desc= 'Post Filter Type', id= 'MFX_LFCOMP_POST_FILT', init= 32769, min= 32768, max= 32770 },
            { desc= 'Post Filter Cutoff', id= 'MFX_LFCOMP_CUTOFF', init= 32781, min= 32768, max= 32784 },
            { desc= 'Low Gain', id= 'MFX_LFCOMP_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_LFCOMP_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_LFCOMP_BAL', init= 32868, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_LFCOMP_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'BIT CRASHER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_BITC_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_BITC_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_BITC_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_BITC_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Sample Rate', id= 'MFX_BITC_SMP_RATE', init= 32848, min= 32768, max= 32895 },
            { desc= 'Bit Down', id= 'MFX_BITC_BIT_DOWN', init= 32784, min= 32768, max= 32788 },
            { desc= 'Filter', id= 'MFX_BITC_FILTER', init= 32895, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_BITC_LO_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_BITC_HI_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_BITC_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'PITCH SHIFTER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_STPS_CTRL_ASGN1', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 2', id= 'MFX_STPS_CTRL_ASGN2', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 3', id= 'MFX_STPS_CTRL_ASGN3', init= 0, min= 0, max= 3 },
            { desc= 'Control Assign 4', id= 'MFX_STPS_CTRL_ASGN4', init= 0, min= 0, max= 3 },
            { desc= 'Coarse', id= 'MFX_STPS_CRS', init= 32792, min= 32768, max= 32804 },
            { desc= 'Fine', id= 'MFX_STPS_FINE', init= 32818, min= 32768, max= 32868 },
            { desc= 'Delay Time (sync sw)', id= 'MFX_STPS_DLY_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Delay Time (msec)', id= 'MFX_STPS_DLY_MSEC', init= 32768, min= 32768, max= 34068 },
            { desc= 'Delay Time (note)', id= 'MFX_STPS_DLY_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Feedback', id= 'MFX_STPS_FBK', init= 32817, min= 32768, max= 32866 },
            { desc= 'Low Gain', id= 'MFX_STPS_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_STPS_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_STPS_BAL', init= 32868, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_STPS_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= '2VOICE PITCH SHIFTER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_TVPS_CTRL_ASGN1', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 2', id= 'MFX_TVPS_CTRL_ASGN2', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 3', id= 'MFX_TVPS_CTRL_ASGN3', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 4', id= 'MFX_TVPS_CTRL_ASGN4', init= 0, min= 0, max= 7 },
            { desc= 'Pitch1 Coarse', id= 'MFX_TVPS_CRS1', init= 32796, min= 32768, max= 32804 },
            { desc= 'Pitch1 Fine', id= 'MFX_TVPS_FINE1', init= 32818, min= 32768, max= 32868 },
            { desc= 'Pitch1 Delay (sync sw)', id= 'MFX_TVPS_DLY1_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Pitch1 Delay (msec)', id= 'MFX_TVPS_DLY1_MSEC', init= 33068, min= 32768, max= 34068 },
            { desc= 'Pitch1 Delay (note)', id= 'MFX_TVPS_DLY1_NOTE', init= 32777, min= 32768, max= 32789 },
            { desc= 'Pitch1 Feedback', id= 'MFX_TVPS_FBK1', init= 32817, min= 32768, max= 32866 },
            { desc= 'Pitch1 Pan', id= 'MFX_TVPS_PAN1', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pitch1 Level', id= 'MFX_TVPS_LEVEL1', init= 32895, min= 32768, max= 32895 },
            { desc= 'Pitch2 Coarse', id= 'MFX_TVPS_CRS2', init= 32799, min= 32768, max= 32804 },
            { desc= 'Pitch2 Fine', id= 'MFX_TVPS_FINE2', init= 32818, min= 32768, max= 32868 },
            { desc= 'Pitch2 Delay (sync sw)', id= 'MFX_TVPS_DLY2_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Pitch2 Delay (msec)', id= 'MFX_TVPS_DLY2_MSEC', init= 33368, min= 32768, max= 34068 },
            { desc= 'Pitch2 Delay (note)', id= 'MFX_TVPS_DLY2_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Pitch2 Feedback', id= 'MFX_TVPS_FBK2', init= 32817, min= 32768, max= 32866 },
            { desc= 'Pitch2 Pan', id= 'MFX_TVPS_PAN2', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pitch2 Level', id= 'MFX_TVPS_LEVEL2', init= 32895, min= 32768, max= 32895 },
            { desc= 'Low Gain', id= 'MFX_TVPS_EQ_LO', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_TVPS_EQ_HI', init= 32783, min= 32768, max= 32798 },
            { desc= 'Balance', id= 'MFX_TVPS_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_TVPS_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'OD->CHORUS',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_ODSCHO_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_ODSCHO_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_ODSCHO_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_ODSCHO_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Overdrive Drive', id= 'MFX_ODSCHO_OD_DRV', init= 32832, min= 32768, max= 32895 },
            { desc= 'Overdrive Pan', id= 'MFX_ODSCHO_OD_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Chorus Pre Delay', id= 'MFX_ODSCHO_CHO_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Chorus Rate (sync sw)', id= 'MFX_ODSCHO_CHO_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Chorus Rate (Hz)', id= 'MFX_ODSCHO_CHO_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Chorus Rate (note)', id= 'MFX_ODSCHO_CHO_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Chorus Depth', id= 'MFX_ODSCHO_CHO_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'Chorus Balance', id= 'MFX_ODSCHO_CHO_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_ODSCHO_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'OD->FLANGER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_ODSFLG_CTRL_ASGN1', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 2', id= 'MFX_ODSFLG_CTRL_ASGN2', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 3', id= 'MFX_ODSFLG_CTRL_ASGN3', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 4', id= 'MFX_ODSFLG_CTRL_ASGN4', init= 0, min= 0, max= 5 },
            { desc= 'Overdrive Drive', id= 'MFX_ODSFLG_OD_DRV', init= 32832, min= 32768, max= 32895 },
            { desc= 'Overdrive Pan', id= 'MFX_ODSFLG_OD_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Flanger Pre Delay', id= 'MFX_ODSFLG_FLG_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Flanger Rate (sync sw)', id= 'MFX_ODSFLG_FLG_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Flanger Rate (Hz)', id= 'MFX_ODSFLG_FLG_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Flanger Rate (note)', id= 'MFX_ODSFLG_FLG_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Flanger Depth', id= 'MFX_ODSFLG_FLG_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Flanger Feedback', id= 'MFX_ODSFLG_FLG_FBK', init= 32847, min= 32768, max= 32866 },
            { desc= 'Flanger Balance', id= 'MFX_ODSFLG_FLG_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_ODSFLG_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'OD->DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_ODSDLY_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_ODSDLY_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_ODSDLY_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_ODSDLY_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Overdrive Drive', id= 'MFX_ODSDLY_OD_DRV', init= 32832, min= 32768, max= 32895 },
            { desc= 'Overdrive Pan', id= 'MFX_ODSDLY_OD_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Delay Time (sync sw)', id= 'MFX_ODSDLY_DLY_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Time (msec)', id= 'MFX_ODSDLY_DLY_MSEC', init= 33368, min= 32768, max= 35368 },
            { desc= 'Delay Time (note)', id= 'MFX_ODSDLY_DLY_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Delay Feedback', id= 'MFX_ODSDLY_DLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'Delay HF Damp', id= 'MFX_ODSDLY_DLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Delay Balance', id= 'MFX_ODSDLY_DLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_ODSDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'DS->CHORUS',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_DSTSCHO_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_DSTSCHO_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_DSTSCHO_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_DSTSCHO_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Distortion Drive', id= 'MFX_DSTSCHO_DST_DRV', init= 32895, min= 32768, max= 32895 },
            { desc= 'Distortion Pan', id= 'MFX_DSTSCHO_DST_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Chorus Pre Delay', id= 'MFX_DSTSCHO_CHO_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Chorus Rate (sync sw)', id= 'MFX_DSTSCHO_CHO_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Chorus Rate (Hz)', id= 'MFX_DSTSCHO_CHO_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Chorus Rate (note)', id= 'MFX_DSTSCHO_CHO_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Chorus Depth', id= 'MFX_DSTSCHO_CHO_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'Chorus Balance', id= 'MFX_DSTSCHO_CHO_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_DSTSCHO_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'DS->FLANGER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_DSTSFLG_CTRL_ASGN1', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 2', id= 'MFX_DSTSFLG_CTRL_ASGN2', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 3', id= 'MFX_DSTSFLG_CTRL_ASGN3', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 4', id= 'MFX_DSTSFLG_CTRL_ASGN4', init= 0, min= 0, max= 5 },
            { desc= 'Distortion Drive', id= 'MFX_DSTSFLG_DST_DRV', init= 32895, min= 32768, max= 32895 },
            { desc= 'Distortion Pan', id= 'MFX_DSTSFLG_DST_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Flanger Pre Delay', id= 'MFX_DSTSFLG_FLG_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Flanger Rate (sync sw)', id= 'MFX_DSTSFLG_FLG_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Flanger Rate (Hz)', id= 'MFX_DSTSFLG_FLG_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Flanger Rate (note)', id= 'MFX_DSTSFLG_FLG_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Flanger Depth', id= 'MFX_DSTSFLG_FLG_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Flanger Feedback', id= 'MFX_DSTSFLG_FLG_FBK', init= 32847, min= 32768, max= 32866 },
            { desc= 'Flanger Balance', id= 'MFX_DSTSFLG_FLG_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_DSTSFLG_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'DS->DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_DSTSDLY_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_DSTSDLY_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_DSTSDLY_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_DSTSDLY_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Distortion Drive', id= 'MFX_DSTSDLY_DST_DRV', init= 32895, min= 32768, max= 32895 },
            { desc= 'Distortion Pan', id= 'MFX_DSTSDLY_DST_PAN', init= 32832, min= 32768, max= 32895 },
            { desc= 'Delay Time (sync sw)', id= 'MFX_DSTSDLY_DLY_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Time (msec)', id= 'MFX_DSTSDLY_DLY_MSEC', init= 33368, min= 32768, max= 35368 },
            { desc= 'Delay Time (note)', id= 'MFX_DSTSDLY_DLY_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Delay Feedback', id= 'MFX_DSTSDLY_DLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'Delay HF Damp', id= 'MFX_DSTSDLY_DLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Delay Balance', id= 'MFX_DSTSDLY_DLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_DSTSDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'OD/DS->TWAH',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_ODTW_CTRL_ASGN1', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 2', id= 'MFX_ODTW_CTRL_ASGN2', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 3', id= 'MFX_ODTW_CTRL_ASGN3', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 4', id= 'MFX_ODTW_CTRL_ASGN4', init= 0, min= 0, max= 6 },
            { desc= 'Drive Switch', id= 'MFX_ODTW_DRIVE_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Drive Type', id= 'MFX_ODTW_DRIVE_TYPE', init= 32768, min= 32768, max= 32769 },
            { desc= 'Drive', id= 'MFX_ODTW_DRIVE', init= 32818, min= 32768, max= 32895 },
            { desc= 'Tone', id= 'MFX_ODTW_TONE', init= 32818, min= 32768, max= 32895 },
            { desc= 'Amp Switch', id= 'MFX_ODTW_AMP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Amp Type', id= 'MFX_ODTW_AMP_TYPE', init= 32768, min= 32768, max= 32771 },
            { desc= 'TWah Switch', id= 'MFX_ODTW_TWAH_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'TWah Mode', id= 'MFX_ODTW_TWAH_MODE', init= 32769, min= 32768, max= 32769 },
            { desc= 'TWah Polarity', id= 'MFX_ODTW_TWAH_POLA', init= 32768, min= 32768, max= 32769 },
            { desc= 'TWah Sens', id= 'MFX_ODTW_TWAH_SENS', init= 32818, min= 32768, max= 32895 },
            { desc= 'TWah Manual', id= 'MFX_ODTW_TWAH_MANUAL', init= 32798, min= 32768, max= 32895 },
            { desc= 'TWah Peak', id= 'MFX_ODTW_TWAH_PEAK', init= 32818, min= 32768, max= 32895 },
            { desc= 'TWah Balance', id= 'MFX_ODTW_TWAH_BAL', init= 32868, min= 32768, max= 32868 },
            { desc= 'Low Gain', id= 'MFX_ODTW_LO_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_ODTW_HI_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_ODTW_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'OD/DS->AWAH',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_ODAW_CTRL_ASGN1', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 2', id= 'MFX_ODAW_CTRL_ASGN2', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 3', id= 'MFX_ODAW_CTRL_ASGN3', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 4', id= 'MFX_ODAW_CTRL_ASGN4', init= 0, min= 0, max= 7 },
            { desc= 'Drive Switch', id= 'MFX_ODAW_DRIVE_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Drive Type', id= 'MFX_ODAW_DRIVE_TYPE', init= 32768, min= 32768, max= 32769 },
            { desc= 'Drive', id= 'MFX_ODAW_DRIVE', init= 32818, min= 32768, max= 32895 },
            { desc= 'Tone', id= 'MFX_ODAW_TONE', init= 32818, min= 32768, max= 32895 },
            { desc= 'Amp Switch', id= 'MFX_ODAW_AMP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Amp Type', id= 'MFX_ODAW_AMP_TYPE', init= 32768, min= 32768, max= 32771 },
            { desc= 'AutoWah Switch', id= 'MFX_ODAW_AWAH_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'AutoWah Mode', id= 'MFX_ODAW_AWAH_MODE', init= 32769, min= 32768, max= 32769 },
            { desc= 'AutoWah Manual', id= 'MFX_ODAW_AWAH_MANUAL', init= 32828, min= 32768, max= 32895 },
            { desc= 'AutoWah Peak', id= 'MFX_ODAW_AWAH_PEAK', init= 32818, min= 32768, max= 32895 },
            { desc= 'AutoWah Rate (sync sw)', id= 'MFX_ODAW_AWAH_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'AutoWah Rate (Hz)', id= 'MFX_ODAW_AWAH_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'AutoWah Rate (note)', id= 'MFX_ODAW_AWAH_RATE_NOTE', init= 32779, min= 32768, max= 32789 },
            { desc= 'AutoWah Depth', id= 'MFX_ODAW_AWAH_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'AutoWah Balance', id= 'MFX_ODAW_AWAH_BAL', init= 32868, min= 32768, max= 32868 },
            { desc= 'Low Gain', id= 'MFX_ODAW_LO_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'High Gain', id= 'MFX_ODAW_HI_GAIN', init= 32783, min= 32768, max= 32798 },
            { desc= 'Level', id= 'MFX_ODAW_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'GTAMPSIM->CHORUS',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_GTCHO_CTRL_ASGN1', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 2', id= 'MFX_GTCHO_CTRL_ASGN2', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 3', id= 'MFX_GTCHO_CTRL_ASGN3', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 4', id= 'MFX_GTCHO_CTRL_ASGN4', init= 0, min= 0, max= 6 },
            { desc= 'Pre Amp Sw', id= 'MFX_GTCHO_AMP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Pre Amp Type', id= 'MFX_GTCHO_AMP_TYPE', init= 32769, min= 32768, max= 32781 },
            { desc= 'Pre Amp Volume', id= 'MFX_GTCHO_AMP_VOLUME', init= 32848, min= 32768, max= 32895 },
            { desc= 'Pre Amp Master', id= 'MFX_GTCHO_AMP_MASTER', init= 32868, min= 32768, max= 32895 },
            { desc= 'Pre Amp Gain', id= 'MFX_GTCHO_AMP_GAIN', init= 32769, min= 32768, max= 32770 },
            { desc= 'Pre Amp Bass', id= 'MFX_GTCHO_AMP_BASS', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pre Amp Middle', id= 'MFX_GTCHO_AMP_MIDDLE', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pre Amp Treble', id= 'MFX_GTCHO_AMP_TREBLE', init= 32832, min= 32768, max= 32895 },
            { desc= 'Speaker Sw', id= 'MFX_GTCHO_SP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Speaker Type', id= 'MFX_GTCHO_SP_TYPE', init= 32772, min= 32768, max= 32783 },
            { desc= 'Chorus Switch', id= 'MFX_GTCHO_CHO_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Chorus Pre Delay', id= 'MFX_GTCHO_CHO_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Chorus Rate (Hz)', id= 'MFX_GTCHO_CHO_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Chorus Depth', id= 'MFX_GTCHO_CHO_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'Chorus Balance', id= 'MFX_GTCHO_CHO_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_GTCHO_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'GTAMPSIM->FLANGER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_GTFLN_CTRL_ASGN1', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 2', id= 'MFX_GTFLN_CTRL_ASGN2', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 3', id= 'MFX_GTFLN_CTRL_ASGN3', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 4', id= 'MFX_GTFLN_CTRL_ASGN4', init= 0, min= 0, max= 7 },
            { desc= 'Pre Amp Sw', id= 'MFX_GTFLN_AMP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Pre Amp Type', id= 'MFX_GTFLN_AMP_TYPE', init= 32769, min= 32768, max= 32781 },
            { desc= 'Pre Amp Volume', id= 'MFX_GTFLN_AMP_VOLUME', init= 32848, min= 32768, max= 32895 },
            { desc= 'Pre Amp Master', id= 'MFX_GTFLN_AMP_MASTER', init= 32868, min= 32768, max= 32895 },
            { desc= 'Pre Amp Gain', id= 'MFX_GTFLN_AMP_GAIN', init= 32769, min= 32768, max= 32770 },
            { desc= 'Pre Amp Bass', id= 'MFX_GTFLN_AMP_BASS', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pre Amp Middle', id= 'MFX_GTFLN_AMP_MIDDLE', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pre Amp Treble', id= 'MFX_GTFLN_AMP_TREBLE', init= 32832, min= 32768, max= 32895 },
            { desc= 'Speaker Sw', id= 'MFX_GTFLN_SP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Speaker Type', id= 'MFX_GTFLN_SP_TYPE', init= 32772, min= 32768, max= 32783 },
            { desc= 'Flanger Switch', id= 'MFX_GTFLN_FLN_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Flanger Pre Delay', id= 'MFX_GTFLN_FLN_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Flanger Rate (Hz)', id= 'MFX_GTFLN_FLN_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Flanger Depth', id= 'MFX_GTFLN_FLN_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Flanger Feedback', id= 'MFX_GTFLN_FLN_FBK', init= 32847, min= 32768, max= 32866 },
            { desc= 'Flanger Balance', id= 'MFX_GTFLN_FLN_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_GTFLN_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'GTAMPSIM->PHASER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_GTPH_CTRL_ASGN1', init= 0, min= 0, max= 8 },
            { desc= 'Control Assign 2', id= 'MFX_GTPH_CTRL_ASGN2', init= 0, min= 0, max= 8 },
            { desc= 'Control Assign 3', id= 'MFX_GTPH_CTRL_ASGN3', init= 0, min= 0, max= 8 },
            { desc= 'Control Assign 4', id= 'MFX_GTPH_CTRL_ASGN4', init= 0, min= 0, max= 8 },
            { desc= 'Pre Amp Sw', id= 'MFX_GTPH_AMP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Pre Amp Type', id= 'MFX_GTPH_AMP_TYPE', init= 32769, min= 32768, max= 32781 },
            { desc= 'Pre Amp Volume', id= 'MFX_GTPH_AMP_VOLUME', init= 32848, min= 32768, max= 32895 },
            { desc= 'Pre Amp Master', id= 'MFX_GTPH_AMP_MASTER', init= 32868, min= 32768, max= 32895 },
            { desc= 'Pre Amp Gain', id= 'MFX_GTPH_AMP_GAIN', init= 32769, min= 32768, max= 32770 },
            { desc= 'Pre Amp Bass', id= 'MFX_GTPH_AMP_BASS', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pre Amp Middle', id= 'MFX_GTPH_AMP_MIDDLE', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pre Amp Treble', id= 'MFX_GTPH_AMP_TREBLE', init= 32832, min= 32768, max= 32895 },
            { desc= 'Speaker Sw', id= 'MFX_GTPH_SP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Speaker Type', id= 'MFX_GTPH_SP_TYPE', init= 32772, min= 32768, max= 32783 },
            { desc= 'Phaser Switch', id= 'MFX_GTPH_PH_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Phaser Rate (Hz)', id= 'MFX_GTPH_PH_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Phaser Manual', id= 'MFX_GTPH_PH_MANUAL', init= 32828, min= 32768, max= 32895 },
            { desc= 'Phaser Depth', id= 'MFX_GTPH_PH_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Phaser Resonance', id= 'MFX_GTPH_PH_RESO', init= 32848, min= 32768, max= 32895 },
            { desc= 'Phaser Mix', id= 'MFX_GTPH_PH_MIX', init= 32895, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_GTPH_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'GTAMPSIM->DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_GTDLY_CTRL_ASGN1', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 2', id= 'MFX_GTDLY_CTRL_ASGN2', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 3', id= 'MFX_GTDLY_CTRL_ASGN3', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 4', id= 'MFX_GTDLY_CTRL_ASGN4', init= 0, min= 0, max= 6 },
            { desc= 'Pre Amp Sw', id= 'MFX_GTDLY_AMP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Pre Amp Type', id= 'MFX_GTDLY_AMP_TYPE', init= 32769, min= 32768, max= 32781 },
            { desc= 'Pre Amp Volume', id= 'MFX_GTDLY_AMP_VOLUME', init= 32848, min= 32768, max= 32895 },
            { desc= 'Pre Amp Master', id= 'MFX_GTDLY_AMP_MASTER', init= 32868, min= 32768, max= 32895 },
            { desc= 'Pre Amp Gain', id= 'MFX_GTDLY_AMP_GAIN', init= 32769, min= 32768, max= 32770 },
            { desc= 'Pre Amp Bass', id= 'MFX_GTDLY_AMP_BASS', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pre Amp Middle', id= 'MFX_GTDLY_AMP_MIDDLE', init= 32832, min= 32768, max= 32895 },
            { desc= 'Pre Amp Treble', id= 'MFX_GTDLY_AMP_TREBLE', init= 32832, min= 32768, max= 32895 },
            { desc= 'Speaker Sw', id= 'MFX_GTDLY_SP_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Speaker Type', id= 'MFX_GTDLY_SP_TYPE', init= 32772, min= 32768, max= 32783 },
            { desc= 'Delay Switch', id= 'MFX_GTDLY_DLY_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Time', id= 'MFX_GTDLY_DLY_TIME', init= 33368, min= 32768, max= 34068 },
            { desc= 'Delay Feedback', id= 'MFX_GTDLY_DLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'Delay HF Damp', id= 'MFX_GTDLY_DLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Delay Balance', id= 'MFX_GTDLY_DLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_GTDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'EP->TREMOLO',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_EPTRM_CTRL_ASGN1', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 2', id= 'MFX_EPTRM_CTRL_ASGN2', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 3', id= 'MFX_EPTRM_CTRL_ASGN3', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 4', id= 'MFX_EPTRM_CTRL_ASGN4', init= 0, min= 0, max= 5 },
            { desc= 'Type', id= 'MFX_EPTRM_TYPE', init= 32768, min= 32768, max= 32770 },
            { desc= 'Bass', id= 'MFX_EPTRM_BASS', init= 32818, min= 32768, max= 32868 },
            { desc= 'Treble', id= 'MFX_EPTRM_TREBLE', init= 32818, min= 32768, max= 32868 },
            { desc= 'Tremolo Switch', id= 'MFX_EPTRM_TRM_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Tremolo Speed (sync sw)', id= 'MFX_EPTRM_TRM_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Tremolo Speed (Hz)', id= 'MFX_EPTRM_TRM_RATE_HZ', init= 32793, min= 32769, max= 32968 },
            { desc= 'Tremolo Speed (note)', id= 'MFX_EPTRM_TRM_RATE_NOTE', init= 32779, min= 32768, max= 32789 },
            { desc= 'Tremolo Depth', id= 'MFX_EPTRM_TRM_DEPTH', init= 32818, min= 32768, max= 32895 },
            { desc= 'Tremolo Duty', id= 'MFX_EPTRM_TRM_DUTY', init= 32776, min= 32768, max= 32788 },
            { desc= 'Speaker Type', id= 'MFX_EPTRM_SPEAKER_TYPE', init= 32769, min= 32768, max= 32772 },
            { desc= 'OD Switch', id= 'MFX_EPTRM_OD_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'OD Gain', id= 'MFX_EPTRM_OD_GAIN', init= 32800, min= 32768, max= 32895 },
            { desc= 'OD Drive', id= 'MFX_EPTRM_OD_DRIVE', init= 32768, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_EPTRM_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'EP->CHORUS',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_EPCHO_CTRL_ASGN1', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 2', id= 'MFX_EPCHO_CTRL_ASGN2', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 3', id= 'MFX_EPCHO_CTRL_ASGN3', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 4', id= 'MFX_EPCHO_CTRL_ASGN4', init= 0, min= 0, max= 6 },
            { desc= 'Type', id= 'MFX_EPCHO_TYPE', init= 32768, min= 32768, max= 32770 },
            { desc= 'Bass', id= 'MFX_EPCHO_BASS', init= 32818, min= 32768, max= 32868 },
            { desc= 'Treble', id= 'MFX_EPCHO_TREBLE', init= 32818, min= 32768, max= 32868 },
            { desc= 'Chorus Switch', id= 'MFX_EPCHO_CHO_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Chorus Pre Delay', id= 'MFX_EPCHO_CHO_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Chorus Rate (sync sw)', id= 'MFX_EPCHO_CHO_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Chorus Rate (Hz)', id= 'MFX_EPCHO_CHO_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Chorus Rate (note)', id= 'MFX_EPCHO_CHO_RATE_NOTE', init= 32779, min= 32768, max= 32789 },
            { desc= 'Chorus Depth', id= 'MFX_EPCHO_CHO_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'Chorus Balance', id= 'MFX_EPCHO_CHO_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Speaker Type', id= 'MFX_EPCHO_SPEAKER_TYPE', init= 32769, min= 32768, max= 32772 },
            { desc= 'OD Switch', id= 'MFX_EPCHO_OD_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'OD Gain', id= 'MFX_EPCHO_OD_GAIN', init= 32800, min= 32768, max= 32895 },
            { desc= 'OD Drive', id= 'MFX_EPCHO_OD_DRIVE', init= 32768, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_EPCHO_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'EP->FLANGER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_EPFLN_CTRL_ASGN1', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 2', id= 'MFX_EPFLN_CTRL_ASGN2', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 3', id= 'MFX_EPFLN_CTRL_ASGN3', init= 0, min= 0, max= 7 },
            { desc= 'Control Assign 4', id= 'MFX_EPFLN_CTRL_ASGN4', init= 0, min= 0, max= 7 },
            { desc= 'Type', id= 'MFX_EPFLN_TYPE', init= 32768, min= 32768, max= 32770 },
            { desc= 'Bass', id= 'MFX_EPFLN_BASS', init= 32818, min= 32768, max= 32868 },
            { desc= 'Treble', id= 'MFX_EPFLN_TREBLE', init= 32818, min= 32768, max= 32868 },
            { desc= 'Flanger Switch', id= 'MFX_EPFLN_FLN_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Flanger Pre Delay', id= 'MFX_EPFLN_FLN_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Flanger Rate (sync sw)', id= 'MFX_EPFLN_FLN_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Flanger Rate (Hz)', id= 'MFX_EPFLN_FLN_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Flanger Rate (note)', id= 'MFX_EPFLN_FLN_RATE_NOTE', init= 32779, min= 32768, max= 32789 },
            { desc= 'Flanger Depth', id= 'MFX_EPFLN_FLN_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'Flanger Feedback', id= 'MFX_EPFLN_FLG_FBK', init= 32847, min= 32768, max= 32866 },
            { desc= 'Flanger Balance', id= 'MFX_EPFLN_FLN_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Speaker Type', id= 'MFX_EPFLN_SPEAKER_TYPE', init= 32769, min= 32768, max= 32772 },
            { desc= 'OD Switch', id= 'MFX_EPFLN_OD_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'OD Gain', id= 'MFX_EPFLN_OD_GAIN', init= 32800, min= 32768, max= 32895 },
            { desc= 'OD Drive', id= 'MFX_EPFLN_OD_DRIVE', init= 32768, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_EPFLN_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'EP->PHASER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_EPPH_CTRL_ASGN1', init= 0, min= 0, max= 8 },
            { desc= 'Control Assign 2', id= 'MFX_EPPH_CTRL_ASGN2', init= 0, min= 0, max= 8 },
            { desc= 'Control Assign 3', id= 'MFX_EPPH_CTRL_ASGN3', init= 0, min= 0, max= 8 },
            { desc= 'Control Assign 4', id= 'MFX_EPPH_CTRL_ASGN4', init= 0, min= 0, max= 8 },
            { desc= 'Type', id= 'MFX_EPPH_TYPE', init= 32768, min= 32768, max= 32770 },
            { desc= 'Bass', id= 'MFX_EPPH_BASS', init= 32818, min= 32768, max= 32868 },
            { desc= 'Treble', id= 'MFX_EPPH_TREBLE', init= 32818, min= 32768, max= 32868 },
            { desc= 'Phaser Switch', id= 'MFX_EPPH_PH_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Phaser Rate (sync sw)', id= 'MFX_EPPH_PH_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Phaser Rate (Hz)', id= 'MFX_EPPH_PH_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Phaser Rate (note)', id= 'MFX_EPPH_PH_RATE_NOTE', init= 32779, min= 32768, max= 32789 },
            { desc= 'Phaser Manual', id= 'MFX_EPPH_PH_MANUAL', init= 32828, min= 32768, max= 32895 },
            { desc= 'Phaser Depth', id= 'MFX_EPPH_PH_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Phaser Resonance', id= 'MFX_EPPH_PH_RESO', init= 32848, min= 32768, max= 32895 },
            { desc= 'Phaser Mix', id= 'MFX_EPPH_PH_MIX', init= 32895, min= 32768, max= 32895 },
            { desc= 'Speaker Type', id= 'MFX_EPPH_SPEAKER_TYPE', init= 32769, min= 32768, max= 32772 },
            { desc= 'OD Switch', id= 'MFX_EPPH_OD_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'OD Gain', id= 'MFX_EPPH_OD_GAIN', init= 32800, min= 32768, max= 32895 },
            { desc= 'OD Drive', id= 'MFX_EPPH_OD_DRIVE', init= 32768, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_EPPH_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'EP->DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_EPDLY_CTRL_ASGN1', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 2', id= 'MFX_EPDLY_CTRL_ASGN2', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 3', id= 'MFX_EPDLY_CTRL_ASGN3', init= 0, min= 0, max= 6 },
            { desc= 'Control Assign 4', id= 'MFX_EPDLY_CTRL_ASGN4', init= 0, min= 0, max= 6 },
            { desc= 'Type', id= 'MFX_EPDLY_TYPE', init= 32768, min= 32768, max= 32770 },
            { desc= 'Bass', id= 'MFX_EPDLY_BASS', init= 32818, min= 32768, max= 32868 },
            { desc= 'Treble', id= 'MFX_EPDLY_TREBLE', init= 32818, min= 32768, max= 32868 },
            { desc= 'Delay Switch', id= 'MFX_EPDLY_DLY_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Time (sync sw)', id= 'MFX_EPDLY_DLY_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Delay Time (msec)', id= 'MFX_EPDLY_DLY_MSEC', init= 33368, min= 32768, max= 34068 },
            { desc= 'Delay Time (note)', id= 'MFX_EPDLY_DLY_NOTE', init= 32779, min= 32768, max= 32789 },
            { desc= 'Delay Acceleration', id= 'MFX_EPDLY_DLY_ACCEL', init= 32778, min= 32768, max= 32783 },
            { desc= 'Delay Feedback', id= 'MFX_EPDLY_DLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'Delay HF Damp', id= 'MFX_EPDLY_DLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Delay Balance', id= 'MFX_EPDLY_DLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Speaker Type', id= 'MFX_EPDLY_SPEAKER_TYPE', init= 32769, min= 32768, max= 32772 },
            { desc= 'OD Switch', id= 'MFX_EPDLY_OD_SW', init= 32769, min= 32768, max= 32769 },
            { desc= 'OD Gain', id= 'MFX_EPDLY_OD_GAIN', init= 32800, min= 32768, max= 32895 },
            { desc= 'OD Drive', id= 'MFX_EPDLY_OD_DRIVE', init= 32768, min= 32768, max= 32895 },
            { desc= 'Level', id= 'MFX_EPDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'ENHANCER->CHORUS',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_EHSCHO_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_EHSCHO_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_EHSCHO_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_EHSCHO_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Enhancer Sens', id= 'MFX_EHSCHO_EH_SENS', init= 32832, min= 32768, max= 32895 },
            { desc= 'Enhancer Mix', id= 'MFX_EHSCHO_EH_MIX', init= 32832, min= 32768, max= 32895 },
            { desc= 'Chorus Pre Delay', id= 'MFX_EHSCHO_CHO_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Chorus Rate (sync sw)', id= 'MFX_EHSCHO_CHO_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Chorus Rate (Hz)', id= 'MFX_EHSCHO_CHO_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Chorus Rate (note)', id= 'MFX_EHSCHO_CHO_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Chorus Depth', id= 'MFX_EHSCHO_CHO_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'Chorus Balance', id= 'MFX_EHSCHO_CHO_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_EHSCHO_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'ENHANCER->FLANGER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_EHSFLG_CTRL_ASGN1', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 2', id= 'MFX_EHSFLG_CTRL_ASGN2', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 3', id= 'MFX_EHSFLG_CTRL_ASGN3', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 4', id= 'MFX_EHSFLG_CTRL_ASGN4', init= 0, min= 0, max= 5 },
            { desc= 'Enhancer Sens', id= 'MFX_EHSFLG_EH_SENS', init= 32832, min= 32768, max= 32895 },
            { desc= 'Enhancer Mix', id= 'MFX_EHSFLG_EH_MIX', init= 32832, min= 32768, max= 32895 },
            { desc= 'Flanger Pre Delay', id= 'MFX_EHSFLG_FLG_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Flanger Rate (sync sw)', id= 'MFX_EHSFLG_FLG_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Flanger Rate (Hz)', id= 'MFX_EHSFLG_FLG_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Flanger Rate (note)', id= 'MFX_EHSFLG_FLG_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Flanger Depth', id= 'MFX_EHSFLG_FLG_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Flanger Feedback', id= 'MFX_EHSFLG_FLG_FBK', init= 32847, min= 32768, max= 32866 },
            { desc= 'Flanger Balance', id= 'MFX_EHSFLG_FLG_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_EHSFLG_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'ENHANCER->DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_EHSDLY_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_EHSDLY_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_EHSDLY_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_EHSDLY_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Enhancer Sens', id= 'MFX_EHSDLY_EH_SENS', init= 32832, min= 32768, max= 32895 },
            { desc= 'Enhancer Mix', id= 'MFX_EHSDLY_EH_MIX', init= 32832, min= 32768, max= 32895 },
            { desc= 'Delay Time (sync sw)', id= 'MFX_EHSDLY_DLY_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Time (msec)', id= 'MFX_EHSDLY_DLY_MSEC', init= 33368, min= 32768, max= 35368 },
            { desc= 'Delay Time (note)', id= 'MFX_EHSDLY_DLY_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Delay Feedback', id= 'MFX_EHSDLY_DLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'Delay HF Damp', id= 'MFX_EHSDLY_DLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Delay Balance', id= 'MFX_EHSDLY_DLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_EHSDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'CHORUS->DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_CHOSDLY_CTRL_ASGN1', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 2', id= 'MFX_CHOSDLY_CTRL_ASGN2', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 3', id= 'MFX_CHOSDLY_CTRL_ASGN3', init= 0, min= 0, max= 4 },
            { desc= 'Control Assign 4', id= 'MFX_CHOSDLY_CTRL_ASGN4', init= 0, min= 0, max= 4 },
            { desc= 'Chorus Pre Delay', id= 'MFX_CHOSDLY_CHO_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Chorus Rate (sync sw)', id= 'MFX_CHOSDLY_CHO_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Chorus Rate (Hz)', id= 'MFX_CHOSDLY_CHO_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Chorus Rate (note)', id= 'MFX_CHOSDLY_CHO_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Chorus Depth', id= 'MFX_CHOSDLY_CHO_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'Chorus Balance', id= 'MFX_CHOSDLY_CHO_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Delay Time (sync sw)', id= 'MFX_CHOSDLY_DLY_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Time (msec)', id= 'MFX_CHOSDLY_DLY_MSEC', init= 33368, min= 32768, max= 35368 },
            { desc= 'Delay Time (note)', id= 'MFX_CHOSDLY_DLY_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Delay Feedback', id= 'MFX_CHOSDLY_DLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'Delay HF Damp', id= 'MFX_CHOSDLY_DLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Delay Balance', id= 'MFX_CHOSDLY_DLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_CHOSDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'FLANGER->DELAY',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_FLGSDLY_CTRL_ASGN1', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 2', id= 'MFX_FLGSDLY_CTRL_ASGN2', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 3', id= 'MFX_FLGSDLY_CTRL_ASGN3', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 4', id= 'MFX_FLGSDLY_CTRL_ASGN4', init= 0, min= 0, max= 5 },
            { desc= 'Flanger Pre Delay', id= 'MFX_FLGSDLY_FLG_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Flanger Rate (sync sw)', id= 'MFX_FLGSDLY_FLG_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Flanger Rate (Hz)', id= 'MFX_FLGSDLY_FLG_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Flanger Rate (note)', id= 'MFX_FLGSDLY_FLG_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Flanger Depth', id= 'MFX_FLGSDLY_FLG_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Flanger Feedback', id= 'MFX_FLGSDLY_FLG_FBK', init= 32847, min= 32768, max= 32866 },
            { desc= 'Flanger Balance', id= 'MFX_FLGSDLY_FLG_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Delay Time (sync sw)', id= 'MFX_FLGSDLY_DLY_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Delay Time (msec)', id= 'MFX_FLGSDLY_DLY_MSEC', init= 33368, min= 32768, max= 35368 },
            { desc= 'Delay Time (note)', id= 'MFX_FLGSDLY_DLY_NOTE', init= 32780, min= 32768, max= 32789 },
            { desc= 'Delay Feedback', id= 'MFX_FLGSDLY_DLY_FBK', init= 32827, min= 32768, max= 32866 },
            { desc= 'Delay HF Damp', id= 'MFX_FLGSDLY_DLY_HF_DAMP', init= 32785, min= 32768, max= 32785 },
            { desc= 'Delay Balance', id= 'MFX_FLGSDLY_DLY_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_FLGSDLY_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    },

    {
        desc= 'CHORUS->FLANGER',
        leaf= {
            { desc= 'Control Assign 1', id= 'MFX_CHOSFLG_CTRL_ASGN1', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 2', id= 'MFX_CHOSFLG_CTRL_ASGN2', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 3', id= 'MFX_CHOSFLG_CTRL_ASGN3', init= 0, min= 0, max= 5 },
            { desc= 'Control Assign 4', id= 'MFX_CHOSFLG_CTRL_ASGN4', init= 0, min= 0, max= 5 },
            { desc= 'Chorus Pre Delay', id= 'MFX_CHOSFLG_CHO_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Chorus Rate (sync sw)', id= 'MFX_CHOSFLG_CHO_RATE_SYNC', init= 32768, min= 32768, max= 32769 },
            { desc= 'Chorus Rate (Hz)', id= 'MFX_CHOSFLG_CHO_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Chorus Rate (note)', id= 'MFX_CHOSFLG_CHO_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Chorus Depth', id= 'MFX_CHOSFLG_CHO_DEPTH', init= 32798, min= 32768, max= 32895 },
            { desc= 'Chorus Balance', id= 'MFX_CHOSFLG_CHO_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Flanger Pre Delay', id= 'MFX_CHOSFLG_FLG_DLY', init= 32788, min= 32768, max= 32893 },
            { desc= 'Flanger Rate (sync sw)', id= 'MFX_CHOSFLG_FLG_RATE_SYNC', init= 32769, min= 32768, max= 32769 },
            { desc= 'Flanger Rate (Hz)', id= 'MFX_CHOSFLG_FLG_RATE_HZ', init= 32778, min= 32769, max= 32968 },
            { desc= 'Flanger Rate (note)', id= 'MFX_CHOSFLG_FLG_RATE_NOTE', init= 32786, min= 32768, max= 32789 },
            { desc= 'Flanger Depth', id= 'MFX_CHOSFLG_FLG_DEPTH', init= 32808, min= 32768, max= 32895 },
            { desc= 'Flanger Feedback', id= 'MFX_CHOSFLG_FLG_FBK', init= 32847, min= 32768, max= 32866 },
            { desc= 'Flanger Balance', id= 'MFX_CHOSFLG_FLG_BAL', init= 32818, min= 32768, max= 32868 },
            { desc= 'Level', id= 'MFX_CHOSFLG_LEVEL', init= 32895, min= 32768, max= 32895 }
        }
    }
}