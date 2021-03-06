#include <boost/make_shared.hpp>

#include <util/Logger.h>
#include "GraphCutDialog.h"

logger::LogChannel graphcutdialoglog("graphcutdialoglog", "[GraphCutDialog] ");

GraphCutDialog::ParametersCollector::ParametersCollector() {

	LOG_ALL(graphcutdialoglog) << "[ParametersCollector] create new parameters collector" << std::endl;

	registerInput(_foregroundPrior,   "foreground prior");
	registerInput(_pottsWeight,       "potts-term weight");
	registerInput(_contrastWeight,    "contrast-term weight");
	registerInput(_contrastSigma,     "contrast-term sigma");
	registerInput(_eightNeighborhood, "eight-neighborhood");
	registerOutput(_parameters,       "parameters");

	_pottsWeight.registerBackwardCallback(&GraphCutDialog::ParametersCollector::onModified, this);
	_pottsWeight.registerBackwardSlot(_update);

	_foregroundPrior.registerBackwardCallback(&GraphCutDialog::ParametersCollector::onModified, this);
	_foregroundPrior.registerBackwardSlot(_update);

	_contrastWeight.registerBackwardCallback(&GraphCutDialog::ParametersCollector::onModified, this);
	_contrastWeight.registerBackwardSlot(_update);

	_contrastSigma.registerBackwardCallback(&GraphCutDialog::ParametersCollector::onModified, this);
	_contrastSigma.registerBackwardSlot(_update);

	_eightNeighborhood.registerBackwardCallback(&GraphCutDialog::ParametersCollector::onModified, this);
	_eightNeighborhood.registerBackwardSlot(_update);

	_parameters.registerForwardCallback(&GraphCutDialog::ParametersCollector::onUpdate, this);
	_parameters.registerForwardSlot(_modified);
	_parameters.registerForwardSlot(_updated);
}

GraphCutDialog::ParametersCollector::~ParametersCollector() {

	LOG_ALL(graphcutdialoglog) << "[ParametersCollector] destructed" << std::endl;
}

void
GraphCutDialog::ParametersCollector::onModified(const pipeline::Modified& signal) {

	LOG_ALL(graphcutdialoglog) << "[ParametersCollector] one of my inputs changed" << std::endl;

	// TODO:
	// • send Modified only if a button is pressed

	_modified();
}

void
GraphCutDialog::ParametersCollector::onUpdate(const pipeline::Update& signal) {

	LOG_ALL(graphcutdialoglog) << "[ParametersCollector] update requested" << std::endl;

	_update();

	_parameters->foregroundPrior   = *_foregroundPrior;
	_parameters->pottsWeight       = *_pottsWeight;
	_parameters->contrastWeight    = *_contrastWeight;
	_parameters->contrastSigma     = *_contrastSigma;
	_parameters->eightNeighborhood = *_eightNeighborhood;

	_updated();

	LOG_ALL(graphcutdialoglog) << "[ParametersCollector] up to date" << std::endl;
}

GraphCutDialog::GraphCutDialog() :
	_foregroundPriorSlider(boost::make_shared<gui::Slider>("foreground prior", 0.0, 1.0)),
	_pottsWeightSlider(boost::make_shared<gui::Slider>("potts-term weight", 0.0, 10.0)),
	_contrastWeightSlider(boost::make_shared<gui::Slider>("contrast-term weight", 0.0, 10.0)),
	_contrastSigmaSlider(boost::make_shared<gui::Slider>("contrast-term sigma", 0.01, 2.0)),
	_eightNeighborhoodSwitch(boost::make_shared<gui::Switch>("eight neighborhood")),
	_gui(boost::make_shared<gui::ContainerView<gui::VerticalPlacing> >()),
	_parametersCollector(boost::make_shared<ParametersCollector>()) {

	LOG_ALL(graphcutdialoglog) << "create new graph cut dialog" << std::endl;

	// establish internal pipeline connections

	_parametersCollector->setInput("foreground prior", _foregroundPriorSlider->getOutput("value"));
	_parametersCollector->setInput("potts-term weight", _pottsWeightSlider->getOutput("value"));
	_parametersCollector->setInput("contrast-term weight", _contrastWeightSlider->getOutput("value"));
	_parametersCollector->setInput("contrast-term sigma", _contrastSigmaSlider->getOutput("value"));
	_parametersCollector->setInput("eight-neighborhood", _eightNeighborhoodSwitch->getOutput("value"));

	_gui->addInput(_foregroundPriorSlider->getOutput("painter"));
	_gui->addInput(_pottsWeightSlider->getOutput("painter"));
	_gui->addInput(_contrastWeightSlider->getOutput("painter"));
	_gui->addInput(_contrastSigmaSlider->getOutput("painter"));
	_gui->addInput(_eightNeighborhoodSwitch->getOutput("painter"));

	// publish internal outputs

	registerOutput(_parametersCollector->getOutput(), "parameters");
	registerOutput(_gui->getOutput(), "painter");
}

GraphCutDialog::~GraphCutDialog() {

	LOG_ALL(graphcutdialoglog) << "destructed" << std::endl;
}
