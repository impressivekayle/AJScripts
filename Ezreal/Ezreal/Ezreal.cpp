#include "PluginSDK.h"




PluginSetup("Ezreal");

IMenu* MainMenu;
IMenu* QMenu;
IMenu* WMenu;
IMenu* RMenu;
IMenu* Misc;
IMenu* Drawings;
IMenuOption* ComboQ;
IMenuOption* ComboW;
IMenuOption* FarmQ;
IMenuOption* ManaFarmQ;
IMenuOption* ComboR;
IMenuOption* ComboRSkillRange;
IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawW;
IMenuOption* DrawR;

ISpell2* Q;
ISpell2* W;
ISpell2* R;

void Menu()
{
	MainMenu = GPluginSDK->AddMenu("Ezreal");
	QMenu = MainMenu->AddMenu("Q Settings");
	WMenu = MainMenu->AddMenu("W Settings");
	RMenu = MainMenu->AddMenu("R Settings");
	Drawings = MainMenu->AddMenu("Drawings");

	ComboQ = QMenu->CheckBox("UseQ", true);
	FarmQ = QMenu->CheckBox("UseQFarm", true);
	ManaFarmQ = QMenu->AddInteger("Mana Manager(%)(AutoQ)", 1, 100, 60);

	ComboW = WMenu->CheckBox("Use W", true);
	


	ComboR = RMenu->CheckBox("Use R", true);
	
	DrawReady = Drawings->CheckBox("Draw only ready spells", true);

	DrawQ = Drawings->CheckBox("Draw Q", true);
	DrawW = Drawings->CheckBox("Draw W", true);
	DrawR = Drawings->CheckBox("Draw R", true);
}

void LoadSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, false, static_cast<eCollisionFlags>(kCollidesWithMinions | kCollidesWithYasuoWall));
	W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, true, true, kCollidesWithNothing);
	R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, false, false, kCollidesWithNothing);
	Q->SetOverrideRange(1150);
	W->SetOverrideRange(1000);
	R->SetOverrideRange(1337000);
	Q->SetOverrideDelay(0.05f);
	W->SetOverrideDelay(0.05f);
	R->SetOverrideDelay(1.00f);
	Q->SetOverrideSpeed(2000);
	W->SetOverrideSpeed(1550);
	R->SetOverrideSpeed(2000);
}

void Combo()
{
	if (ComboQ->Enabled())
	{
		if (Q->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
			Q->CastOnTarget(target, kHitChanceMedium);

		}
	}
	if (ComboW->Enabled())
	{
		if (W->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, W->Range());
			W->CastOnTarget(target, kHitChanceMedium);
		}
	}
	if (ComboR->Enabled())
	{
		if (R->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());
			auto dmg = GHealthPrediction->GetKSDamage(target, kSlotW, R->GetDelay(), true);

			if (dmg > target->GetHealth())
			{
				if (R->CastOnTarget(target, kHitChanceHigh))
					return;
			}
		}
	}
}



void Farm()
{
	if (GEntityList->Player()->ManaPercent() >= ManaFarmQ->GetInteger())
	{
		if (FarmQ->Enabled() && Q->IsReady())
			Q->LastHitMinion();
	}
}
	

PLUGIN_EVENT(void) OnRender()
{

	if (DrawReady->Enabled())

	{

		if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

					

		if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }



		if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }



	}

	else

	{

		if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }


				
		if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }



		if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }

	}

}

PLUGIN_EVENT(void) OnGameUpdate()

{

	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)

	{

		Combo();

	}



	if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)

	{

		Farm();

	}

}

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)

{



	PluginSDKSetup(PluginSDK);

	Menu();

	LoadSpells();





	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);

	GEventManager->AddEventHandler(kEventOnRender, OnRender);



}


PLUGIN_API void OnUnload()

{

	MainMenu->Remove();





	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);

	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);



}

