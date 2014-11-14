/* Copyright 2013-2014 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <QObject>
#include <memory>

#include "Setting.h"

/*!
 * \brief A setting that 'overrides another.'
 * This means that the setting's default value will be the value of another setting.
 * The other setting can be (and usually is) a part of a different SettingsObject
 * than this one.
 *
 * If this setting is given a non-null default value that default value will be used, rather
 * than that of the other setting.
 */
class OverrideSetting : public Setting
{
	Q_OBJECT
public:
	explicit OverrideSetting(std::shared_ptr<Setting> other, const QVariant &defValue);

	virtual QVariant defValue() const;

protected:
	std::shared_ptr<Setting> m_other;
};
