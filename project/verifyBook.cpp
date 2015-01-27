#include "verifybook.h"
#include <iostream>
#include "inventory.h"



verifyBook::verifyBook(){
	std::cerr << "VERIFY BOOK" << std::endl;
}

void verifyBook::check(std::string check,metsparserContext& context){

	if ( check.compare("1")!=0 ) return; 
	init (context);

	// check if BIBREC_SYSNUM in database
	std::string BIBREC_SYSNUM = context.inventory.inventoryMODSMD_ELEC.BIBREC_SYS_NUM;
	if (!db->getInventory(BIBREC_SYSNUM,invent)){
		hError -> getError(cat_bookinventory,"METS","DMDSEC MOD_SMD_ELEC", "BIBREC_SYSNUM not found:" + BIBREC_SYSNUM ,context.metsfile,"");
		return; // skip all
	};



	if ( invent.BIBREC_245a.compare(context.inventory.inventoryMODSMD_ELEC.BIBREC_245a)) {
		hError -> getError(cat_bookinventoryTitle,"METS","DMDSEC MOD_SMD_ELEC", "wrong title:" + context.inventory.inventoryMODSMD_ELEC.BIBREC_245a ,context.metsfile,"");
	}

	if ( invent.BIBREC_245b.compare(context.inventory.inventoryMODSMD_ELEC.BIBREC_245b)) {
		hError -> getError(cat_bookinventoryTitle,"METS","DMDSEC MOD_SMD_ELEC", "wrong title:" + context.inventory.inventoryMODSMD_ELEC.BIBREC_245b ,context.metsfile,"");
	}

	if ( invent.BIBREC_245a.compare(context.inventory.inventoryMODSMD_PRINT.BIBREC_245a)) {
		hError -> getError(cat_bookinventoryTitle,"METS","DMDSEC MOD_SMD_PRINT", "wrong title:" + context.inventory.inventoryMODSMD_PRINT.BIBREC_245a ,context.metsfile,"");
	}

	if ( invent.BIBREC_245b.compare(context.inventory.inventoryMODSMD_PRINT.BIBREC_245b)) {
		hError -> getError(cat_bookinventoryTitle,"METS","DMDSEC MOD_SMD_PRINT", "wrong title:" + context.inventory.inventoryMODSMD_PRINT.BIBREC_245b ,context.metsfile,"");
	}

	if ( invent.BIBREC_100a_1.compare(context.inventory.inventoryMODSMD_ELEC.BIBREC_100a_1)) {
		hError -> getError(cat_bookinventoryData,"METS","DMDSEC MOD_SMD_ELEC", "wrong BIBREC_100a_1:" + context.inventory.inventoryMODSMD_ELEC.BIBREC_100a_1 ,context.metsfile,"");
	}

	if ( invent.BIBREC_100a_2.compare(context.inventory.inventoryMODSMD_ELEC.BIBREC_100a_2)) {
		hError -> getError(cat_bookinventoryData,"METS","DMDSEC MOD_SMD_ELEC", "wrong BIBREC_100a_2:" + context.inventory.inventoryMODSMD_ELEC.BIBREC_100a_2 ,context.metsfile,"");
	}

	if ( invent.BIBREC_100a_1.compare(context.inventory.inventoryMODSMD_PRINT.BIBREC_100a_1)) {
		hError -> getError(cat_bookinventoryData,"METS","DMDSEC MOD_SMD_PRINT", "wrong BIBREC_100a_1:" + context.inventory.inventoryMODSMD_PRINT.BIBREC_100a_1 ,context.metsfile,"");
	}

	if ( invent.BIBREC_100a_2.compare(context.inventory.inventoryMODSMD_PRINT.BIBREC_100a_2)) {
		hError -> getError(cat_bookinventoryData,"METS","DMDSEC MOD_SMD_PRINT", "wrong BIBREC_100a_2:" + context.inventory.inventoryMODSMD_PRINT.BIBREC_100a_2 ,context.metsfile,"");
	}
	
	if ( invent.languageTerm.compare(context.inventory.inventoryMODSMD_ELEC.languageTerm)) {
		hError -> getError(cat_bookinventoryData,"METS","DMDSEC MOD_SMD_ELEC", "wrong languageTerm:" + context.inventory.inventoryMODSMD_ELEC.languageTerm ,context.metsfile,"");
	}

	if ( invent.languageTerm.compare(context.inventory.inventoryMODSMD_PRINT.languageTerm)) {
		hError -> getError(cat_bookinventoryData,"METS","DMDSEC MOD_SMD_PRINT", "wrong languageTerm:" + context.inventory.inventoryMODSMD_PRINT.languageTerm ,context.metsfile,"");
	}

	if ( invent.ITEM_barcode.compare(context.inventory.inventoryMODSMD_PRINT.BIBREC_SYS_NUM /* put there in the xml */ )) {
		hError -> getError(cat_bookinventoryData,"METS","DMDSEC MOD_SMD_PRINT", "wrong languageTerm:" + context.inventory.inventoryMODSMD_PRINT.BIBREC_SYS_NUM ,context.metsfile,"");
	}

	if ( invent.BIBREC_260b.compare(context.inventory.inventoryMODSMD_PRINT.BIBREC_260b  )) {
		hError -> getError(cat_bookinventoryData,"METS","DMDSEC MOD_SMD_PRINT", "wrong BIBREC_260b:" + context.inventory.inventoryMODSMD_PRINT.BIBREC_260b ,context.metsfile,"");
	}

	if ( invent.BIBREC_260c.compare(context.inventory.inventoryMODSMD_PRINT.BIBREC_260c  )) {
		hError -> getError(cat_bookinventoryData,"METS","DMDSEC MOD_SMD_PRINT", "wrong BIBREC_260c:" + context.inventory.inventoryMODSMD_PRINT.BIBREC_260c ,context.metsfile,"");
	}

	if ( invent.BIBREC_SYS_NUM.compare(context.inventory.inventoryMARCMD_ALEPHSYNC.BIBREC_SYS_NUM )) {
		hError -> getError(cat_bookinventoryData,"METS","DMDSEC MOD_SMD_ALEPHSYNC", "wrong BIBREC_SYS_NUM:" + context.inventory.inventoryMARCMD_ALEPHSYNC.BIBREC_SYS_NUM ,context.metsfile,"");
	}

}

void verifyBook::init(metsparserContext& context){
	hError = context.hError;
	db = hError -> getDatabase();


}