#include "SopnetDialog.h"

logger::LogChannel sopnetdialoglog("sopnetdialoglog", "[SopnetDialog] ");

SopnetDialog::SopnetDialog() :
	_forceExplanationSwitch(boost::make_shared<gui::Switch>("force slice explanation")),
	_segmentationCostWeightSlider(boost::make_shared<gui::Slider>("segmentation cost weight", 0.0, 100.0, 1.0)),
	_segmentationCostPottsWeightSlider(boost::make_shared<gui::Slider>("segmentation cost potts weight", -100, 100, 1.0)),
	_segmentationCostPriorForegroundSlider(boost::make_shared<gui::Slider>("segmentation cost prior foreground", 0, 1, 0.5)),
	_priorEndSlider(boost::make_shared<gui::Slider>("prior end", -100000, 100000.0, 0.0)),
	_priorContinuationSlider(boost::make_shared<gui::Slider>("prior continuation", -100000.0, 100000.0, 0.0)),
	_priorBranchSlider(boost::make_shared<gui::Slider>("prior branch", -100000.0, 100000.0, 0.0)),
	_containerView(boost::make_shared<gui::ContainerView<gui::VerticalPlacing> >()),
	_parameterAssembler(boost::make_shared<ParameterAssembler>()) {

	registerOutput(_forceExplanationSwitch->getOutput("value"), "force explanation");
	registerOutput(_parameterAssembler->getOutput("segmentation cost parameters"), "segmentation cost parameters");
	registerOutput(_parameterAssembler->getOutput("prior cost parameters"), "prior cost parameters");
	registerOutput(_containerView->getOutput(), "painter");

	_parameterAssembler->setInput("segmentation cost weight", _segmentationCostWeightSlider->getOutput("value"));
	_parameterAssembler->setInput("segmentation cost potts weight", _segmentationCostPottsWeightSlider->getOutput("value"));
	_parameterAssembler->setInput("segmentation cost prior foreground", _segmentationCostPriorForegroundSlider->getOutput("value"));
	_parameterAssembler->setInput("prior end", _priorEndSlider->getOutput("value"));
	_parameterAssembler->setInput("prior continuation", _priorContinuationSlider->getOutput("value"));
	_parameterAssembler->setInput("prior branch", _priorBranchSlider->getOutput("value"));

	_containerView->addInput(_forceExplanationSwitch->getOutput("painter"));
	_containerView->addInput(_segmentationCostWeightSlider->getOutput("painter"));
	_containerView->addInput(_segmentationCostPottsWeightSlider->getOutput("painter"));
	_containerView->addInput(_segmentationCostPriorForegroundSlider->getOutput("painter"));
	_containerView->addInput(_priorEndSlider->getOutput("painter"));
	_containerView->addInput(_priorContinuationSlider->getOutput("painter"));
	_containerView->addInput(_priorBranchSlider->getOutput("painter"));
}

SopnetDialog::ParameterAssembler::ParameterAssembler() {

	registerInput(_segmentationCostWeight, "segmentation cost weight");
	registerInput(_segmentationCostPottsWeight, "segmentation cost potts weight");
	registerInput(_segmentationCostPriorForeground, "segmentation cost prior foreground");
	registerInput(_priorEnd, "prior end");
	registerInput(_priorContinuation, "prior continuation");
	registerInput(_priorBranch, "prior branch");

	registerOutput(_segmentationCostFunctionParameters, "segmentation cost parameters");
	registerOutput(_priorCostFunctionParameters, "prior cost parameters");
}

void
SopnetDialog::ParameterAssembler::updateOutputs() {

	LOG_ALL(sopnetdialoglog) << "updating parameters" << std::endl;

	_segmentationCostFunctionParameters->weight          = *_segmentationCostWeight;
	_segmentationCostFunctionParameters->weightPotts     = *_segmentationCostPottsWeight;
	_segmentationCostFunctionParameters->priorForeground = *_segmentationCostPriorForeground;

	_priorCostFunctionParameters->priorEnd          = *_priorEnd;
	_priorCostFunctionParameters->priorContinuation = *_priorContinuation;
	_priorCostFunctionParameters->priorBranch       = *_priorBranch;
}
