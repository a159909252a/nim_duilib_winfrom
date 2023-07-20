#include "AnimationManager.h"
#include "duilib/Core/Control.h"

namespace ui 
{

AnimationManager::AnimationManager() :
	m_pControl(nullptr)
{
}

AnimationPlayer* AnimationManager::GetAnimationPlayer(AnimationType animationType) const
{
	auto it = m_animationMap.find(animationType);
	if (it != m_animationMap.end()) {
		return it->second.get();
	}
	else {
		return nullptr;
	}
}

AnimationPlayer* AnimationManager::SetFadeHot(bool bFadeHot)
{
	AnimationPlayer* animationArgs = nullptr;
	if (bFadeHot) {
		animationArgs = new AnimationPlayer();
		animationArgs->SetStartValue(0);
		animationArgs->SetEndValue(255);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedDownRatio(0.7);
		animationArgs->SetTotalMillSeconds(50);//控件处于焦点状态时，动画效果的总时长
		auto playCallback = nbase::Bind(&Control::SetHotAlpha, m_pControl, std::placeholders::_1);
		animationArgs->SetCallback(playCallback);
		m_animationMap[kAnimationHot].reset(animationArgs);
	}
	else {
		m_animationMap.erase(kAnimationHot);
	}

	return animationArgs;
}

AnimationPlayer* AnimationManager::SetFadeAlpha(bool bFadeVisible)
{
	AnimationPlayer* animationArgs = nullptr;
	if (bFadeVisible) {
		animationArgs = new AnimationPlayer();
		animationArgs->SetStartValue(0);
		animationArgs->SetEndValue(255);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedDownRatio(0.7);
		animationArgs->SetTotalMillSeconds(250);
		auto playCallback = nbase::Bind(&Control::SetAlpha, m_pControl, std::placeholders::_1);
		animationArgs->SetCallback(playCallback);
		m_animationMap[kAnimationAlpha].reset(animationArgs);
		m_pControl->SetAlpha(0);
	}
	else {
		m_animationMap.erase(kAnimationAlpha);
		m_pControl->SetAlpha(255);
	}

	return animationArgs;
}

AnimationPlayer* AnimationManager::SetFadeWidth(bool bFadeWidth)
{
	AnimationPlayer* animationArgs = nullptr;
	int32_t cx = 0;
	if (bFadeWidth) {
		UiEstSize estSize = m_pControl->EstimateSize(UiSize(999999, 999999));
		cx = estSize.cx.GetInt32();
		ASSERT(cx > 0);
	}
	if (bFadeWidth && (cx > 0)) {
		animationArgs = new AnimationPlayer();
		animationArgs->SetStartValue(0);
		animationArgs->SetEndValue(cx);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedUpfactorA(0.00084);
		animationArgs->SetSpeedDownRatio(0.7);
		auto playCallback = nbase::Bind(&Control::SetFixedWidth64, m_pControl, std::placeholders::_1);
		animationArgs->SetCallback(playCallback);
		m_animationMap[kAnimationWidth].reset(animationArgs);
	}
	else {
		m_animationMap.erase(kAnimationWidth);
	}

	return animationArgs;
}

AnimationPlayer* AnimationManager::SetFadeHeight(bool bFadeHeight)
{
	AnimationPlayer* animationArgs = nullptr;
	int32_t cy = 0;
	if (bFadeHeight) {
		UiEstSize estSize = m_pControl->EstimateSize(UiSize(999999, 999999));
		cy = estSize.cy.GetInt32();
		ASSERT(cy > 0);
	}
	if (bFadeHeight && (cy > 0)) {
		animationArgs = new AnimationPlayer();
		animationArgs->SetStartValue(0);
		animationArgs->SetEndValue(cy);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedUpfactorA(0.00084);
		animationArgs->SetSpeedDownRatio(0.7);
		auto playCallback = nbase::Bind(&Control::SetFixedHeight64, m_pControl, std::placeholders::_1);
		animationArgs->SetCallback(playCallback);
		m_animationMap[kAnimationHeight].reset(animationArgs);
	}
	else {
		m_animationMap.erase(kAnimationHeight);
	}

	return animationArgs;
}

AnimationPlayer* AnimationManager::SetFadeInOutX(bool bFade, bool bIsFromRight)
{
	AnimationPlayer* animationArgs = nullptr;
	int32_t cx = 0;
	if (bFade) {
		UiEstSize estSize = m_pControl->EstimateSize(UiSize(999999, 999999));
		cx = estSize.cx.GetInt32();
		if (cx <= 0) {
			cx = 100;
		}
	}
	if (bFade) {
		animationArgs = new AnimationPlayer();
		animationArgs->SetEndValue(0);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedUpfactorA(0.006);
		animationArgs->SetSpeedDownRatio(0.7);
		auto playCallback = nbase::Bind(&Control::SetRenderOffsetX, m_pControl, std::placeholders::_1);
		animationArgs->SetCallback(playCallback);

		if (bIsFromRight) {
			animationArgs->SetStartValue(-cx);
			m_animationMap[kAnimationInoutXFromRight].reset(animationArgs);
		}
		else {
			animationArgs->SetStartValue(cx);
			m_animationMap[kAnimationInoutXFromLeft].reset(animationArgs);
		}
	}
	else{
		if (bIsFromRight) {
			m_animationMap.erase(kAnimationInoutXFromRight);
		}
		else {
			m_animationMap.erase(kAnimationInoutXFromLeft);
		}
	}

	return animationArgs;
}

AnimationPlayer* AnimationManager::SetFadeInOutY(bool bFade, bool bIsFromBottom)
{
	AnimationPlayer* animationArgs = nullptr;
	int32_t cy = 0;
	if (bFade) {
		UiEstSize estSize = m_pControl->EstimateSize(UiSize(999999, 999999));
		cy = estSize.cy.GetInt32();
		if (cy <= 0) {
			cy = 100;
		}
	}
	if (bFade) {
		animationArgs = new AnimationPlayer();
		animationArgs->SetEndValue(0);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedUpfactorA(0.006);
		animationArgs->SetSpeedDownRatio(0.7);
		auto playCallback = nbase::Bind(&Control::SetRenderOffsetY, m_pControl, std::placeholders::_1);
		animationArgs->SetCallback(playCallback);

		if (bIsFromBottom) {
			animationArgs->SetStartValue(-cy);
			m_animationMap[kAnimationInoutYFromBottom].reset(animationArgs);
		}
		else {
			animationArgs->SetStartValue(cy);
			m_animationMap[kAnimationInoutYFromTop].reset(animationArgs);
		}
	}
	else{
		if (bIsFromBottom) {
			m_animationMap.erase(kAnimationInoutYFromBottom);
		}
		else {
			m_animationMap.erase(kAnimationInoutYFromTop);
		}
	}

	return animationArgs;
}

void AnimationManager::Appear()
{
	m_pControl->SetVisible(true);
	if (GetAnimationPlayer(kAnimationAlpha)) {
		m_animationMap[kAnimationAlpha]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationAlpha]->Continue();
	}
	if (GetAnimationPlayer(kAnimationWidth)) {
		m_animationMap[kAnimationWidth]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationWidth]->Continue();
	}
	if (GetAnimationPlayer(kAnimationHeight)) {
		m_animationMap[kAnimationHeight]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationHeight]->Continue();
	}
	if (GetAnimationPlayer(kAnimationInoutXFromLeft)) {
		m_animationMap[kAnimationInoutXFromLeft]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationInoutXFromLeft]->Continue();
	}
	if (GetAnimationPlayer(kAnimationInoutXFromRight)) {
		m_animationMap[kAnimationInoutXFromRight]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationInoutXFromRight]->Continue();
	}
	if (GetAnimationPlayer(kAnimationInoutYFromTop)) {
		m_animationMap[kAnimationInoutYFromTop]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationInoutYFromTop]->Continue();
	}
	if (GetAnimationPlayer(kAnimationInoutYFromBottom)) {
		m_animationMap[kAnimationInoutYFromBottom]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationInoutYFromBottom]->Continue();
	}
}

void AnimationManager::Disappear()
{
	bool handled = false;

	CompleteCallback completeCallback = nbase::Bind(&Control::SetVisible, m_pControl, false);
	if (GetAnimationPlayer(kAnimationAlpha)) {
		m_animationMap[kAnimationAlpha]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationAlpha]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationWidth)) {
		m_animationMap[kAnimationWidth]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationWidth]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationHeight)) {
		m_animationMap[kAnimationHeight]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationHeight]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationInoutXFromLeft)) {
		m_animationMap[kAnimationInoutXFromLeft]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationInoutXFromLeft]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationInoutXFromRight)) {
		m_animationMap[kAnimationInoutXFromRight]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationInoutXFromRight]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationInoutYFromTop)) {
		m_animationMap[kAnimationInoutYFromTop]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationInoutYFromTop]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationInoutYFromBottom)) {
		m_animationMap[kAnimationInoutYFromBottom]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationInoutYFromBottom]->ReverseContinue();
		handled = true;
	}

	if (!handled) {
		m_pControl->SetVisible(false);
	}
}

void AnimationManager::MouseEnter()
{
	if (GetAnimationPlayer(kAnimationHot)) {
		m_animationMap[kAnimationHot]->Continue();
	}
}

void AnimationManager::MouseLeave()
{
	if (GetAnimationPlayer(kAnimationHot)) {
		m_animationMap[kAnimationHot]->ReverseContinue();
	}
}

void AnimationManager::Clear(Control* control)
{
	if (m_pControl != nullptr) {
		ASSERT(control == m_pControl);
	}

	for (auto& iter : m_animationMap) {
		if (iter.second != nullptr) {
			iter.second->Clear();
		}
	}

	m_pControl = nullptr;
	m_animationMap.clear();
}

}